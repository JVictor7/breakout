//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <cstdio>
#include<fstream>
#include<sstream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdlib>     /* srand, rand */
#include <ctime>
/*Codigo de utilizacao no jogo seaquest do atari
autoria: igor de souza, joshp zica e joao victor;
 */
//Screen dimension constants
#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT  730
#define velocidadex 4
#define velocidadey 4
#define velocidadeBolax 3 
#define velocidadeBolay 3
#define qtxtijolo 9
#define qtytijolo 5
using namespace std;
//Structure of object

class StrTextura {
    //Image
    SDL_Texture* textura;
    //Dimensions and position
public:

    bool loadTExtureFromFile(char * path, SDL_Renderer* gRenderer1) {
        //The final texture
        SDL_Texture* newTexture = NULL;
        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        } else {
            //Color key image
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(gRenderer1, loadedSurface);
            if (newTexture == NULL) {
                printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            }
            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }
        //Return success
        SetTextura(newTexture);
        return (GetTextura() != NULL);
    }

    SDL_Texture* GetTextura() {
        return textura;
    }

    void SetTextura(SDL_Texture* textura) {
        this->textura = textura;
    }
    //Dimensions and position
};

class StrPosition {
    int velX;
    int velY;
    SDL_Rect posicao;
public:

    int GetPosicaoX() {
        return posicao.x;
    }

    int GetPosicaoY() {
        return posicao.y;
    }

    int GetPosicaoW() {
        return posicao.w;
    }

    int GetPosicaoH() {
        return posicao.h;
    }

    SDL_Rect& GetPosicaoEndereco() {
        return posicao;
    }

    SDL_Rect GetPosicao() {
        return posicao;
    }

    void SetPosicao(SDL_Rect posicao) {
        this->posicao = posicao;
    }

    void SetPosicaoX(int x) {
        this->posicao.x = x;
    }

    void SetPosicaoY(int y) {
        this->posicao.y = y;
    }

    void SetPosicaoW(int w) {
        this->posicao.w = w;
    }

    void SetPosicaoH(int h) {
        this->posicao.h = h;
    }

    int GetVelX() {
        return velX;
    }

    void SetVelX(int velX) {
        this->velX = velX;
    }

    int GetVelY() {
        return velY;
    }

    void SetVelY(int velY) {
        this->velY = velY;
    }
};

class PLATAFORMA {
public:
    StrPosition posicao;
    StrTextura textura;

    void InicializaPlataforma() {
        posicao.SetPosicaoW(185);
        posicao.SetPosicaoH(30);
        posicao.SetPosicaoX(320);
        posicao.SetPosicaoY(620);
        posicao.SetVelY(0);
        posicao.SetVelX(0);
    }

    bool loadMediaPlataforma(SDL_Renderer* gRenderer1) {
        if (!textura.loadTExtureFromFile("plataforma.png", gRenderer1)) {
            printf("Failed to load arrow texture!\n");
            return false;
        }
        return true;
    }

    void RenderizaPlataforma(SDL_Renderer* gRenderer1) {
        SDL_RenderCopyEx(gRenderer1, textura.GetTextura(), NULL, &(posicao.GetPosicaoEndereco()), 0, NULL, SDL_FLIP_NONE);
    }

    void TrataPosicaoPlataforma() {
        int x = posicao.GetPosicaoX();
        int w = posicao.GetPosicaoW();
        int y = posicao.GetPosicaoY();
        int h = posicao.GetPosicaoH();
        int ok = 1;
        if ((x + posicao.GetVelX() * velocidadex) > SCREEN_WIDTH - w) {
            ok = 0;
        }
        if ((x + posicao.GetVelX() * velocidadex) < 0) {
            ok = 0;
        }
        if ((y - posicao.GetVelY() * velocidadey) > SCREEN_HEIGHT - h) {
            ok = 0;
        }
        if ((y + posicao.GetVelY() * velocidadey) < 0) {
            ok = 0;
        }
        //Atualiza o mundo
        if (ok) {
            posicao.SetPosicaoX(posicao.GetPosicaoX() + posicao.GetVelX() * velocidadex);
            posicao.SetPosicaoY(posicao.GetPosicaoY() + posicao.GetVelY() * velocidadey);
        }
    }
};

class TEXTO {
public:
    
    const char *PontuacaoTxt;
    SDL_Color fontColor;
    SDL_Surface *textSurface;
    SDL_Texture *texto;

    //The event structure
    SDL_Event event;

    //The font that's going to be used

    //The color of the font
    SDL_Color textColor = {255, 255, 255};

    bool loadMediaTexto() {


        if (TTF_Init() == -1) {
            return false;
        }


        return true;
    }
    void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
    {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
    }

    bool load_ttf(SDL_Renderer *gRenderer1, int contagemDePontos) {
        //Open the font
        TTF_Font *fontePalavras = TTF_OpenFont("Century Gothic.ttf", 36);

        ///------Conversão de inteiro para "const char*"------
        stringstream strs;
        strs << contagemDePontos;
        string temp_str = strs.str();
        PontuacaoTxt = (char*) temp_str.c_str();
        ///---------------------------------------------------
        char textoTxt[25];
        strcpy(textoTxt, "ELEMENTOS RESTANTES: ");
        strcat(textoTxt, PontuacaoTxt);
        fontColor = {255, 255, 255, 255};
        

        textSurface = TTF_RenderText_Blended(fontePalavras, textoTxt, fontColor);
        texto = SDL_CreateTextureFromSurface(gRenderer1, textSurface);

        SDL_Rect textoRect;
        textoRect.x = 320;
        textoRect.y = 670;

        SDL_QueryTexture(texto, NULL, NULL, &textoRect.w, &textoRect.h);
        SDL_RenderCopy(gRenderer1, texto, NULL, &textoRect);

        return true;
    }

    //If everything loaded fine

    void finalizarTexto() { // Finaliza todos os textos renderizados na tela

        SDL_FreeSurface(textSurface);
        textSurface = NULL;
        SDL_DestroyTexture(texto);
        texto = NULL;
    }
};

class MUSICA {
public:
    Mix_Chunk *SomBola = NULL;
    Mix_Chunk *SomDerr = NULL;
    Mix_Chunk *SomOxigenio = NULL;

    bool loadMediaAudio() {
        SomBola = Mix_LoadWAV("Sons/BateTijolo.mp3");
        SomDerr = Mix_LoadWAV("Sons/Derrota.mp3");
        return true;
    }

    bool iniciarSons(int opcao) { // Inicia sons durante o jogo, tendo momentos especificos para cada um tocar.

        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            return false;
            cout << "Erro" << endl;
        } else {
            Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 512);

            if (opcao == 1) {
                Mix_PlayChannel(-1, SomBola, 0);
                if (SomBola == NULL) {
                    cout << "Mix_LoadMUS('Sons/BateTijolo.mp3'): " << Mix_GetError() << endl;
                    return false;
                }
            }

            if (opcao == 2) {
                Mix_PlayChannel(-1, SomDerr, 0);
                if (SomDerr == NULL) {
                    cout << "Mix_LoadMUS('Sons/Derrota.mp3'): " << Mix_GetError() << endl;
                    return false;
                }
            }

            return true;
        }
    }

    void closeAudio() {
        Mix_FreeChunk(SomBola);
        Mix_FreeChunk(SomDerr);
        SomBola = NULL;

    }
};

class BOLA {
public:
    StrPosition posicao;
    StrTextura textura;
    int vidas = 5;

    void InicializaBola() {
        posicao.SetPosicaoW(19);
        posicao.SetPosicaoH(19);
        posicao.SetPosicaoX(320);
        posicao.SetPosicaoY(490);
        posicao.SetVelY(-1);
        posicao.SetVelX(-1);
    }

    bool loadMediaBola(SDL_Renderer* gRenderer1) {
        if (!textura.loadTExtureFromFile("bola1.png", gRenderer1)) {
            printf("Failed to load arrow texture!\n");
            return false;
        }
        return true;
    }

    void RenderizaBola(SDL_Renderer* gRenderer1) {
        SDL_RenderCopyEx(gRenderer1, textura.GetTextura(), NULL, &(posicao.GetPosicaoEndereco()), 0, NULL, SDL_FLIP_NONE);
    }

    void TrataPosicaoBola() {
        MUSICA musica;
        bool success;
        musica.loadMediaAudio();

        int x = posicao.GetPosicaoX();
        int w = posicao.GetPosicaoW();
        int y = posicao.GetPosicaoY();
        int h = posicao.GetPosicaoH();
        if (x > SCREEN_WIDTH - w) {
            posicao.SetVelX(-1);
            posicao.SetPosicaoX(SCREEN_WIDTH - w);
        }
        if (x < 0) {
            posicao.SetVelX(1);
            posicao.SetPosicaoX(0);
        }
        if (y > SCREEN_HEIGHT - h) {
            InicializaBola();
            musica.iniciarSons(2);
        }
        if (y < 0) {
            posicao.SetPosicaoY(0);
            posicao.SetVelY(1);
        }
        //Atualiza o mundo
        posicao.SetPosicaoX(posicao.GetPosicaoX() + posicao.GetVelX() * velocidadeBolax);
        posicao.SetPosicaoY(posicao.GetPosicaoY() + posicao.GetVelY() * velocidadeBolay);
    }

};

class MENU {
public:
    StrPosition menuposicao, winp, losp;
    StrTextura texturamenu, wint, lost;

    void InicializaMenu() {
        menuposicao.SetPosicaoW(1000);
        menuposicao.SetPosicaoH(730);
        menuposicao.SetPosicaoX(1);
        menuposicao.SetPosicaoY(1);
        winp.SetPosicaoW(1000);
        winp.SetPosicaoH(730);
        winp.SetPosicaoX(1);
        winp.SetPosicaoY(1);
        losp.SetPosicaoW(1000);
        losp.SetPosicaoH(730);
        losp.SetPosicaoX(1);
        losp.SetPosicaoY(1);
    }

    void RenderizaMENU(SDL_Renderer* gRenderer1) {
        SDL_RenderClear(gRenderer1);
        SDL_RenderCopyEx(gRenderer1, texturamenu.GetTextura(), NULL, &(menuposicao.GetPosicaoEndereco()), 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer1);
    }

    bool loadMediaMenu(SDL_Renderer* gRenderer1) {
        if (!texturamenu.loadTExtureFromFile("menu.png", gRenderer1)) {
            printf("Failed to load arrow texture!\n");
            return false;
        }
        if (!wint.loadTExtureFromFile("win.png", gRenderer1)) {
            printf("Failed to load arrow texture!\n");
            return false;
        }
        if (!wint.loadTExtureFromFile("lose.png", gRenderer1)) {
            printf("Failed to load arrow texture!\n");
            return false;
        }
        return true;
    }

    void RenderizaWIN(SDL_Renderer* gRenderer1) {
        SDL_RenderClear(gRenderer1);
        SDL_RenderCopyEx(gRenderer1, wint.GetTextura(), NULL, &(winp.GetPosicaoEndereco()), 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer1);
    }

    void RenderizaLOS(SDL_Renderer* gRenderer1) {
        SDL_RenderClear(gRenderer1);
        SDL_RenderCopyEx(gRenderer1, lost.GetTextura(), NULL, &(losp.GetPosicaoEndereco()), 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer1);
    }

    int Start() {
        SDL_Event e;
        int a = 0;
        while (a != 1 || a != 2) {
            SDL_PollEvent(&e);
            if (e.type == SDL_QUIT) {
                exit(0);
            }
            if (e.key.keysym.sym == SDLK_SPACE && e.key.state == SDL_PRESSED) {
                a = 1;
                return 1;
            }
        }
    }

    void Fim() {
        SDL_Event e;
        int a = 0;
        while (a != 1 || a != 2) {
            SDL_PollEvent(&e);
            if (e.type == SDL_QUIT) {
                exit(0);
            }
            if (e.key.keysym.sym == SDLK_SPACE && e.key.state == SDL_PRESSED) {
                a = 1;
                exit(0);
            }
        }
    }
};

class TIJOLO {
public:
    StrPosition posicao[qtytijolo][qtxtijolo];
    StrTextura textura;
    bool existe[qtytijolo][qtxtijolo];

    void InicializaTijolo() {
        int k = 1, g = 1;
        posicao[0][0].SetPosicaoW(80);
        posicao[0][0].SetPosicaoH(80);
        int w = posicao[0][0].GetPosicaoW();
        int h = posicao[0][0].GetPosicaoH();
        for (int i = 0; i < qtytijolo; i++) {
            for (int j = 0; j < qtxtijolo; j++) {
                existe[i][j] = true;
                posicao[i][j].SetPosicaoY(i * h + h * k / 6 + 55);
                posicao[i][j].SetPosicaoX(j * w + w * g / 6 + 55);
                posicao[i][j].SetPosicaoW(80);
                posicao[i][j].SetPosicaoH(80);
                posicao[i][j].SetVelY(0);
                posicao[i][j].SetVelX(0);

                g++;
            }
            g = 1;
            k++;
        }
    }

    bool loadMediaTijolo(SDL_Renderer* gRenderer1) {
        if (!textura.loadTExtureFromFile("plataforma.png", gRenderer1)) {
            printf("Failed to load arrow texture!\n");
            return false;
        }
        return true;
    }

    void RenderizaTijolo(SDL_Renderer* gRenderer1) {
        for (int i = 0; i < qtytijolo; i++) {
            for (int j = 0; j < qtxtijolo; j++) {
                if (existe[i][j] == true) {
                    SDL_RenderCopyEx(gRenderer1, textura.GetTextura(), NULL, &(posicao[i][j].GetPosicaoEndereco()), 0, NULL, SDL_FLIP_NONE);
                }
            }
        }
    }
};

class PONTO {
    int x, y;
public:

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }

    int GetX() {
        return x;
    }

    int GetY() {
        return y;
    }
};

class Jogo {
public:
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Renderer* Renderer = NULL;
    PLATAFORMA plataforma;
    MENU menu;
    MUSICA musica;
    BOLA bola;
    TIJOLO tijolo;
    TEXTO texto;
    int tijolosdestruidos = 0;

    Jogo() {
        srand(time(NULL));
        //Mix_PlayChannel(-1, musica.SomNasc, 0);
        if (!init()) {
            printf("Failed to initialize!\n");
        } else {
            //Load media
            if (!loadMediaObjetos()) {
                printf("Failed to load media!\n");
            } else {
                InicializaObjetos();
                menu.RenderizaMENU(gRenderer);
                if (menu.Start() == 1) {
                    gameLoop();
                }
            }
        }
        close();
    }

    bool init() {
        //Initialization flag
        bool success = true;
        //Initialize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Set texture filtering to linear
            if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
                printf("Warning: Linear texture filtering not enabled!");
            }
            //Create window
            gWindow = SDL_CreateWindow("SDL GAME SEAQUEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (gWindow == NULL) {
                printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Create vsynced renderer for window
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if (gRenderer == NULL) {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags)) {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                        success = false;
                    }
                }
            }
        }
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return false;
        }
        return success;
    }

    bool loadMediaObjetos() {
        bool success = true;
        if (!(menu.loadMediaMenu(gRenderer))) {
            success = false;
        }
        if (!(musica.loadMediaAudio())) {
            success = false;
        }
        if (!(texto.load_ttf(gRenderer, 10))) {
            success = false;
        }
        if (!(plataforma.loadMediaPlataforma(gRenderer))) {
            success = false;
        }
        if (!(bola.loadMediaBola(gRenderer))) {
            success = false;
        }
        if (!(tijolo.loadMediaTijolo(gRenderer))) {
            success = false;
        }


        return success;
    }

    void TrataObjetos() {
        plataforma.TrataPosicaoPlataforma();
        for (int b = 0; b < velocidadeBolax; b++) {
            bola.TrataPosicaoBola();
            if (ColisaoBola()) {
                break;

            }
        }
    }

    void InicializaObjetos() {
        menu.InicializaMenu();
        tijolo.InicializaTijolo();
        plataforma.InicializaPlataforma();
        bola.InicializaBola();
        texto.loadMediaTexto();



    }

    void trataEventos(bool * quit) {
        //Event handler
        SDL_Event e;
        //Handle events on queue                
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                *quit = true;
            } else {
                if (!(e.key.state == SDL_PRESSED || e.type == SDL_KEYDOWN)) {
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:
                            plataforma.posicao.SetVelX(0);
                            break;
                        case SDLK_RIGHT:
                            plataforma.posicao.SetVelX(0);
                            break;
                        case SDLK_UP:
                            plataforma.posicao.SetVelY(0);
                            break;
                        case SDLK_DOWN:
                            plataforma.posicao.SetVelY(0);
                            break;
                    }
                } else {
                    if (e.key.state == SDL_PRESSED) {
                        switch (e.key.keysym.sym) {
                            case SDLK_LEFT:
                                plataforma.posicao.SetVelX(-1);
                                break;
                            case SDLK_RIGHT:
                                plataforma.posicao.SetVelX(1);
                                break;
                            case SDLK_UP:
                                plataforma.posicao.SetVelY(0);
                                break;
                            case SDLK_DOWN:
                                plataforma.posicao.SetVelY(0);
                                break;
                        }
                    }
                }
            }
        }
    }

    bool gameLoop() {
        int g = 0;
        bool quit = false;
        
        while (!quit) {
            trataEventos(&quit);
            TrataObjetos();
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            RenderizaObjetos();
            if (tijolosdestruidos == 15) {
                menu.RenderizaWIN(gRenderer);
                menu.Fim();

            }
            if (bola.vidas == 0) {
                menu.RenderizaLOS(gRenderer);
                menu.Fim();
            }
        }
    }

    void RenderizaObjetos() {
        SDL_RenderClear(gRenderer);
        tijolo.RenderizaTijolo(gRenderer);
        bola.RenderizaBola(gRenderer);
        plataforma.RenderizaPlataforma(gRenderer);
        SDL_RenderPresent(gRenderer);
    }

    void ColisaoPlataforma() {
        int xp = plataforma.posicao.GetPosicaoX();
        int yp = plataforma.posicao.GetPosicaoY();
        int wp = plataforma.posicao.GetPosicaoW();
        int hp = plataforma.posicao.GetPosicaoH();
        int xb = bola.posicao.GetPosicaoX();
        int yb = bola.posicao.GetPosicaoY();
        int wb = bola.posicao.GetPosicaoW();
        int hb = bola.posicao.GetPosicaoH();
        if ((yb + hb) >= yp && (xb + wb) >= xp && xb <= (xp + wp)&&(yb + hb)<(yp + hp)) {
            bola.posicao.SetVelY(-1);
            musica.iniciarSons(1);
            if ((xb + wb / 2)>(xp + wp / 2)) {
                bola.posicao.SetVelX(1);
                musica.iniciarSons(1);
            } else {
                bola.posicao.SetVelX(-1);
            }
        }
    }

    bool ColisaoTijolo() {

        int xb = bola.posicao.GetPosicaoX();
        int yb = bola.posicao.GetPosicaoY();
        int wb = bola.posicao.GetPosicaoW();
        int hb = bola.posicao.GetPosicaoH();
        int xt;
        int yt;
        int wt;
        int ht;

        for (int i = 0; i < qtytijolo; i++) {
            for (int j = 0; j < qtxtijolo; j++) {

                xt = tijolo.posicao[i][j].GetPosicaoX();
                yt = tijolo.posicao[i][j].GetPosicaoY();
                wt = tijolo.posicao[i][j].GetPosicaoW();
                ht = tijolo.posicao[i][j].GetPosicaoH();

                if ((xb + wb) >= xt && xb <= (xt + wt)&&(yb) <= (yt + ht)&&(yb + hb) >= (yt)) {
                    PONTO pontoE;
                    PONTO pontoC;
                    PONTO pontoB;
                    PONTO pontoD;
                    PONTO centro;

                    centro.setX(xb + wb / 2);
                    centro.setY(yb + hb / 2);

                    pontoE.setX(xt);
                    pontoE.setY(yt + ht / 2);

                    pontoC.setX(xt + wt / 2);
                    pontoC.setY(yt);

                    pontoD.setX(xt + wt);
                    pontoD.setY(yt + ht / 2);

                    pontoB.setX(xt + wt / 2);
                    pontoB.setY(yt + ht);

                    float DE;
                    float DC;
                    float DD;
                    float DB;

                    DE = sqrt(pow((centro.GetX() - pontoE.GetX()), 2.0) + pow((centro.GetY() - pontoE.GetY()), 2.0));
                    DC = sqrt(pow((centro.GetX() - pontoC.GetX()), 2.0) + pow((centro.GetY() - pontoC.GetY()), 2.0));
                    DD = sqrt(pow((centro.GetX() - pontoD.GetX()), 2.0) + pow((centro.GetY() - pontoD.GetY()), 2.0));
                    DB = sqrt(pow((centro.GetX() - pontoB.GetX()), 2.0) + pow((centro.GetY() - pontoB.GetY()), 2.0));

                    if (bola.posicao.GetVelX() == 1) {
                        if (bola.posicao.GetVelY() == 1) {
                            if (DC < DE) {
                                bola.posicao.SetVelY(bola.posicao.GetVelY()*(-1));
                                musica.iniciarSons(1);
                            } else {
                                bola.posicao.SetVelX(bola.posicao.GetVelX()*(-1));
                                musica.iniciarSons(1);
                            }
                        } else {
                            if (DE < DB) {
                                bola.posicao.SetVelX(bola.posicao.GetVelX()*(-1));
                                musica.iniciarSons(1);
                            } else {
                                bola.posicao.SetVelY(bola.posicao.GetVelY()*(-1));
                                musica.iniciarSons(1);
                            }
                        }
                    } else {
                        if (bola.posicao.GetVelY() == 1) {
                            if (DD < DC) {
                                bola.posicao.SetVelX(bola.posicao.GetVelX()*(-1));
                                musica.iniciarSons(1);
                            } else {
                                bola.posicao.SetVelY(bola.posicao.GetVelY()*(-1));
                                musica.iniciarSons(1);
                            }
                        } else {
                            if (DD < DB) {
                                bola.posicao.SetVelX(bola.posicao.GetVelX()*(-1));
                                musica.iniciarSons(1);
                            } else {
                                bola.posicao.SetVelY(bola.posicao.GetVelY()*(-1));
                                musica.iniciarSons(1);
                            }
                        }
                    }

                    bola.posicao.SetPosicaoX(bola.posicao.GetPosicaoX() + bola.posicao.GetVelX());
                    bola.posicao.SetPosicaoY(bola.posicao.GetPosicaoY() + bola.posicao.GetVelY());
                    tijolo.posicao[i][j].SetPosicaoX(-100);
                    tijolo.posicao[i][j].SetPosicaoY(-100);
                    tijolo.existe[i][j] = false;
                    tijolosdestruidos += 1;
                    return true;
                }

            }
        }
        return false;
    }

    bool ColisaoBola() {
        ColisaoPlataforma();
        if (ColisaoTijolo()) {
            return true;
        } else {
            return false;
        }
    };

    void close() {
        //Free loaded images
        //Destroy window	
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        gRenderer = NULL;
        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }
};

int main(int argc, char* args[]) {
    Jogo jogo;
    return 42;
}
