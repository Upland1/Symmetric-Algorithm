// crypto_gui_sfml26.cpp - Compatible con SFML 2.6.1
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <cstring>

typedef unsigned long long u64;

// -----------------------------
// Operaciones bitwise / util
// -----------------------------
u64 ROL(u64 data, int shift) { return (data << shift) | (data >> (64 - shift)); }
u64 ROR(u64 data, int shift) { return (data >> shift) | (data << (64 - shift)); }

std::string to_hex(u64 value) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::setw(16) << std::setfill('0') << (value);
    return oss.str();
}

std::string to_hex_32(u64 value32) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::setw(8) << std::setfill('0') << static_cast<uint32_t>(value32 & 0xFFFFFFFFULL);
    return oss.str();
}

// -----------------------------
// Estructura visual de un paso
// -----------------------------
struct VisualStep {
    std::string title;
    std::string subclave_hex;
    std::string before_hex;
    std::string L_hex;
    std::string R_hex;
    std::string L_rot_hex;
    std::string R_rot_hex;
    std::string xor_hex;
    std::string added_hex;
    std::string out_hex;
    std::string plaintext; // <-- para mostrar texto plano final
};

// -----------------------------
// Generación subclaves
// -----------------------------
std::vector<u64> generar_subclaves(const std::string& clave_maestra, std::vector<VisualStep>& steps) {
    u64 K0 = *reinterpret_cast<const u64*>(clave_maestra.substr(0, 8).data());
    u64 K1 = *reinterpret_cast<const u64*>(clave_maestra.substr(8, 8).data());

    std::vector<u64> subclaves;
    subclaves.push_back(K0);
    subclaves.push_back(K1);

    {
        VisualStep s;
        s.title = "Generacion subclave 0/1";
        s.subclave_hex = to_hex(K0) + " , " + to_hex(K1);
        steps.push_back(s);
    }

    for (int i = 2; i < 8; ++i) {
        u64 nueva = ROL(subclaves[i - 1], (i * 3) % 64) ^ subclaves[i - 2];
        nueva += 0x9E3779B97F4A7C15ULL;
        subclaves.push_back(nueva);

        VisualStep s;
        s.title = "Generando subclave " + std::to_string(i);
        s.subclave_hex = to_hex(nueva);
        s.before_hex = "prev: " + to_hex(subclaves[i-1]) + " xor prev2: " + to_hex(subclaves[i-2]);
        steps.push_back(s);
    }
    return subclaves;
}

// -----------------------------
// Construye pasos visuales para 1 ronda (cifrado o descifrado)
// -----------------------------
VisualStep build_round_visual(u64 input, u64 subk, int r, bool cifrar) {
    VisualStep s;
    s.title = (cifrar ? "Ronda " : "Ronda (desc) ") + std::to_string(r);
    s.subclave_hex = to_hex(subk);
    s.before_hex = to_hex(input);

    if (cifrar) {
        u64 C1 = input ^ subk;
        int d = subk & 0x3FUL;
        u64 L = C1 & 0xFFFFFFFF00000000ULL;
        u64 R = C1 & 0x00000000FFFFFFFFULL;
        u64 Ld = ROL(L, d);
        u64 Rd = ROR(R, (64 - d));
        u64 D1 = Ld | Rd;
        u64 out = D1 + subk;

        s.L_hex = to_hex_32(L >> 32);
        s.R_hex = to_hex_32(R);
        s.L_rot_hex = to_hex(Ld);
        s.R_rot_hex = to_hex(Rd);
        s.xor_hex = to_hex(C1);
        s.added_hex = to_hex(D1);
        s.out_hex = to_hex(out);
    } else {
        u64 D1 = input - subk;
        int d = subk & 0x3FUL;
        u64 Ld = D1 & 0xFFFFFFFF00000000ULL;
        u64 Rd = D1 & 0x00000000FFFFFFFFULL;
        u64 L = ROR(Ld, d);
        u64 R = ROL(Rd, (64 - d));
        u64 C1 = L | R;
        u64 out = C1 ^ subk;

        s.L_hex = to_hex_32(L >> 32);
        s.R_hex = to_hex_32(R);
        s.L_rot_hex = to_hex(L);
        s.R_rot_hex = to_hex(R);
        s.xor_hex = to_hex(C1);
        s.added_hex = to_hex(D1);
        s.out_hex = to_hex(out);
    }
    return s;
}

// -----------------------------
// Convierte u64 a string quitando padding '\0'
std::string u64_to_plain(u64 v) {
    char buf[9];
    std::memset(buf, 0, 9);
    std::memcpy(buf, &v, 8);
    // quitar bytes nulos al final
    std::string s(buf, buf + 8);
    size_t pos = s.find('\0');
    if (pos != std::string::npos) s.resize(pos);
    return s;
}

std::string wrapText(const std::string& str, sf::Font& font, unsigned int charSize, float maxWidth) {
    std::string result;
    std::string line;

    sf::Text temp("", font, charSize);

    for (char c : str) {
        line += c;
        temp.setString(line);

        if (temp.getLocalBounds().width > maxWidth) {
            result += '\n';
            line = c;
        }
        result += c;
    }
    return result;
}

// -----------------------------
// MAIN GUI
// -----------------------------
int main() {
    std::cout << "Iniciando programa CRUX-8 Visual SFML 2.6.1..." << std::endl;
    
    std::cout << "Creando ventana..." << std::endl;
    sf::RenderWindow window(sf::VideoMode(1100u, 700u), "CRUX-8 Visual - SFML 2.6.1");
    window.setFramerateLimit(60);
    std::cout << "Ventana creada exitosamente." << std::endl;

    // --- fuentes ---
    sf::Font font;
    sf::Font defaultFont; // Fuente de respaldo
    bool fontLoaded = false;
    std::vector<std::string> tryFonts = {
        "./CourierNew.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\cour.ttf",
        "C:\\Windows\\Fonts\\consola.ttf",
        "C:\\Windows\\Fonts\\calibri.ttf"
    };
    for(auto &p : tryFonts) {
        std::cout << "Intentando cargar fuente: " << p << std::endl;
        if(font.loadFromFile(p)) { 
            fontLoaded = true; 
            std::cout << "Fuente cargada exitosamente: " << p << std::endl;
            break; 
        }
    }
    if(!fontLoaded) { 
        std::cerr << "No se pudo cargar ninguna fuente. Usando fuente predeterminada..." << std::endl;
        // Crear una fuente vacía como respaldo
        font = defaultFont;
    }

    // --- textos base ---
    sf::Text title("CRUX-8 Visual - Escriba clave y texto", font, 20u);
    title.setPosition(sf::Vector2f(20.f,10.f)); title.setFillColor(sf::Color::White);

    sf::RectangleShape keyBox(sf::Vector2f(520.f,36.f)); keyBox.setPosition(sf::Vector2f(20.f,55.f)); keyBox.setFillColor(sf::Color(40,40,50));
    sf::RectangleShape textBox(sf::Vector2f(520.f,36.f)); textBox.setPosition(sf::Vector2f(20.f,115.f)); textBox.setFillColor(sf::Color(40,40,50));
    sf::Text keyLabel("Clave (16 chars):", font, 16u); keyLabel.setPosition(sf::Vector2f(20.f,35.f)); keyLabel.setFillColor(sf::Color(200,200,220));
    sf::Text textLabel("Texto (<=8 chars):", font, 16u); textLabel.setPosition(sf::Vector2f(20.f,95.f)); textLabel.setFillColor(sf::Color(200,200,220));

    std::string keyInput; keyInput.reserve(32);
    std::string msgInput; msgInput.reserve(128);
    bool inKey = true;

    sf::Text keyText("", font, 18u); keyText.setPosition(sf::Vector2f(28.f,63.f)); keyText.setFillColor(sf::Color::White);
    sf::Text msgText("", font, 18u); msgText.setPosition(sf::Vector2f(28.f,123.f)); msgText.setFillColor(sf::Color::White);

    // --- panels ---
    sf::RectangleShape subkeyPanel(sf::Vector2f(520.f,410.f)); subkeyPanel.setPosition(sf::Vector2f(560.f,60.f)); subkeyPanel.setFillColor(sf::Color(28,28,34));
    sf::Text subkeyTitle("Subclaves", font, 18u); subkeyTitle.setPosition(sf::Vector2f(560.f,35.f)); subkeyTitle.setFillColor(sf::Color::White);
    sf::RectangleShape roundPanel(sf::Vector2f(520.f,410.f)); roundPanel.setPosition(sf::Vector2f(20.f,170.f)); roundPanel.setFillColor(sf::Color(22,22,28));
    sf::Text roundTitle("Estado", font, 18u); roundTitle.setPosition(sf::Vector2f(20.f,150.f)); roundTitle.setFillColor(sf::Color::White);

    sf::Text hint("ENTER = iniciar | SPACE = siguiente | BACKSPACE = anterior | A = autoplay | ESC = salir", font, 14u);
    hint.setPosition(sf::Vector2f(20.f,650.f)); hint.setFillColor(sf::Color(180,180,200));

    // --- datos y pasos ---
    std::vector<VisualStep> steps;
    std::vector<u64> subclaves;
    bool prepared = false;
    bool playing = false;
    int curStep = 0;
    float scrollOffset = 0.f;
    auto lastTick = std::chrono::steady_clock::now();
    const int autoplay_ms = 500;
    u64 block = 0;

    std::cout << "Entrando al bucle principal..." << std::endl;

    while(window.isOpen()) {
        // --- EVENTS ---
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window request
            if (event.type == sf::Event::Closed) {
                window.close();
                continue;
            }

            // Key pressed events
            if (event.type == sf::Event::KeyPressed) {
                auto code = event.key.code;

                // Escape -> close
                if (code == sf::Keyboard::Escape) {
                    window.close();
                }
                // Tab -> switch input focus
                else if (code == sf::Keyboard::Tab) {
                    inKey = !inKey;
                }
                // Enter -> prepare / run
                else if (code == sf::Keyboard::Enter) {
                    // --- lógica de cifrado/descifrado ---
                    if(keyInput.size()==16 && msgInput.size()<=8){
                        steps.clear();
                        subclaves = generar_subclaves(keyInput,steps);

                        char buf[8]; std::memset(buf,0,8);
                        std::memcpy(buf,msgInput.data(),msgInput.size());
                        block = *reinterpret_cast<u64*>(buf);

                        VisualStep s; s.title="Bloque inicial"; s.before_hex=to_hex(block);
                        // insert after generation info (preserve initial gen step at index 0)
                        steps.insert(steps.begin()+1,s);

                        // cifrado
                        u64 cur = block;
                        for(int r=0;r<8;r++){
                            VisualStep vs = build_round_visual(cur,subclaves[r],r,true);
                            steps.push_back(vs);
                            u64 C1 = cur^subclaves[r];
                            int d = subclaves[r]&0x3FUL;
                            u64 L = C1 & 0xFFFFFFFF00000000ULL;
                            u64 R = C1 & 0x00000000FFFFFFFFULL;
                            u64 Ld = ROL(L,d);
                            u64 Rd = ROR(R,64-d);
                            u64 D1 = Ld|Rd;
                            cur = D1+subclaves[r];
                        }
                        s.title="Resultado cifrado"; s.out_hex=to_hex(cur); steps.push_back(s);

                        // descifrado
                        u64 curd = cur;
                        steps.push_back(VisualStep{"--- Inicia descifrado ---","","","","","","","","",""});
                        for(int r=7;r>=0;r--){
                            VisualStep vs = build_round_visual(curd,subclaves[r],r,false);
                            steps.push_back(vs);
                            u64 D1 = curd - subclaves[r];
                            int d = subclaves[r]&0x3FUL;
                            u64 Ld = D1 & 0xFFFFFFFF00000000ULL;
                            u64 Rd = D1 & 0x00000000FFFFFFFFULL;
                            u64 L = ROR(Ld,d);
                            u64 R = ROL(Rd,64-d);
                            curd = (L|R) ^ subclaves[r];
                        }
                        s.title="Resultado descifrado"; s.out_hex=to_hex(curd); s.plaintext=u64_to_plain(curd); steps.push_back(s);

                        prepared=true; playing=false; curStep=0; scrollOffset=0.f;
                    } else std::cerr << "Clave debe ser 16 chars y texto <=8 bytes.\n";
                }
                // Space -> siguiente (si preparado)
                else if (code == sf::Keyboard::Space && prepared && curStep < (int)steps.size()-1) {
                    curStep++;
                }
                // Backspace -> borrar o retroceder
                else if (code == sf::Keyboard::BackSpace) {
                    if(!prepared){
                        if(inKey && !keyInput.empty()) keyInput.pop_back();
                        else if(!inKey && !msgInput.empty()) msgInput.pop_back();
                    } else if(curStep>0) {
                        curStep--;
                    }
                }
                // A -> autoplay toggle
                else if (code == sf::Keyboard::A && prepared) {
                    playing = !playing;
                    lastTick = std::chrono::steady_clock::now();
                }
                // Up / Down -> scroll
                else if (code == sf::Keyboard::Up) scrollOffset += 20.f;
                else if (code == sf::Keyboard::Down) scrollOffset -= 20.f;
            }

            // Text entered (for typing characters into inputs) - only when not prepared
            if (!prepared) {
                if (event.type == sf::Event::TextEntered) {
                    uint32_t unicode = event.text.unicode;
                    if (unicode >= 32 && unicode <= 126) {
                        char c = static_cast<char>(unicode);
                        if (inKey && keyInput.size()<16) keyInput.push_back(c);
                        else if (!inKey && msgInput.size()<8) msgInput.push_back(c);
                    }
                }
            }
        } // end event loop

        // --- autoplay ---
        if(playing && prepared){
            auto now = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastTick).count();
            if(diff>=autoplay_ms && curStep<(int)steps.size()-1){ curStep++; lastTick=now; }
        }

        // --- actualizar input text ---
        keyText.setString(keyInput + (inKey?"|":""));
        msgText.setString(msgInput + (!inKey?"|":""));

        // --- DRAW ---
        window.clear(sf::Color(14,14,20));
        window.draw(title); window.draw(keyLabel); window.draw(textLabel);
        window.draw(keyBox); window.draw(textBox); window.draw(keyText); window.draw(msgText);
        window.draw(subkeyPanel); window.draw(roundPanel);
        window.draw(subkeyTitle); window.draw(roundTitle); window.draw(hint);

        // subclaves
        sf::Text sktext("", font, 16u);
        sktext.setFillColor(sf::Color(220,220,240));
        sktext.setPosition(sf::Vector2f(subkeyPanel.getPosition().x+12.f, subkeyPanel.getPosition().y+10.f));
        if(!prepared) sktext.setString("Subclaves apareceran aqui despues de ENTER");
        else {
            std::ostringstream oss;
            for(size_t i=0;i<subclaves.size();i++) oss<<"k["<<i<<"]="<<to_hex(subclaves[i])<<"\n";
            sktext.setString(oss.str());
        }
        window.draw(sktext);

        // panel round info
        sf::Text stitle("", font, 18u); stitle.setFillColor(sf::Color::White);
        stitle.setPosition(sf::Vector2f(roundPanel.getPosition().x+12.f, roundPanel.getPosition().y+10.f));
        sf::Text sbody("", font, 15u); sbody.setFillColor(sf::Color(200,200,220));

        if(!prepared){
            stitle.setString("Aun no preparado");
            sbody.setString("Escriba clave (16 chars) y mensaje (<=8 bytes). Presione ENTER para iniciar.");
        } else {
            VisualStep &vs = steps[curStep];
            stitle.setString(vs.title);
            std::ostringstream b;
            if(!vs.before_hex.empty()) b<<"Entrada: "<<vs.before_hex<<"\n";
            if(!vs.subclave_hex.empty()) b<<"Subclave: "<<vs.subclave_hex<<"\n\n";
            if(!vs.L_hex.empty() || !vs.R_hex.empty()){
                b<<"L (high32): "<<vs.L_hex<<"   R (low32): "<<vs.R_hex<<"\n";
                b<<"L rot: "<<vs.L_rot_hex<<"\nR rot: "<<vs.R_rot_hex<<"\n";
                b<<"XOR/comb: "<<vs.xor_hex<<"\nAfter rotate (D1): "<<vs.added_hex<<"\n";
                b<<"Output: "<<vs.out_hex<<"\n";
            } else if(!vs.out_hex.empty()) b<<"Output: "<<vs.out_hex<<"\n";
            if(!vs.plaintext.empty()) b<<"\nTexto plano (descifrado): "<<vs.plaintext<<"\n";
            float maxWidth = roundPanel.getSize().x - 24.f; // un poco menos por padding
            sbody.setString(wrapText(b.str(), font, 15, maxWidth));
        }

        // scroll
        sbody.setPosition(sf::Vector2f(roundPanel.getPosition().x+12.f, roundPanel.getPosition().y+40.f+scrollOffset));
        float panelHeight = roundPanel.getSize().y;
        // SFML 2.6.1 usa .width y .height
        float textHeight = sbody.getLocalBounds().height;
        if(scrollOffset>0.f) scrollOffset=0.f;
        if(textHeight+scrollOffset>panelHeight-20.f) scrollOffset = panelHeight-textHeight-20.f;

        window.draw(stitle);
        window.draw(sbody);

        // footer
        sf::Text footer(("Paso "+std::to_string(curStep+1)+" / "+std::to_string(prepared?steps.size():0)), font, 14u);
        footer.setPosition(sf::Vector2f(900.f,670.f)); footer.setFillColor(sf::Color(180,180,200));
        window.draw(footer);

        window.display();
    }

    std::cout << "Programa terminado correctamente." << std::endl;
    return 0;
}