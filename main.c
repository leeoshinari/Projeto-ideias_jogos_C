#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// variavel global do indice da frase
int indice_frase = 0;

void pre_dialogo(bool* apertado, bool* digitado, Rectangle chat_box, char* frase, Vector2 position, float* x, float* running_time, float update_time);
void dialogo(char* frase, int tamanho_frase_max, float x, int y, float* running_time, float update_time, float max_width_chat_box, float x_inicial);

int main()
{
    // declarando as medidas da janela
    int screenHeight = 800; 
    int screenWitdh = 800; 

    //frase teste, a frase que sera mostrada na tela (pode ser alterada quando quiser)
    char frase[200] = {"Ola, tudo bem com voce"};

    // medidas e posição do botao na janela
    Rectangle botao;
    botao.height = 100;
    botao.width = 100;
    botao.x = screenWitdh/2;
    botao.y = 100;

    // as medidas da caixa de dialogo
    Rectangle chat_box;
    chat_box.height = 400;
    chat_box.width = 800;
    chat_box.x = 0;
    chat_box.y = 400;

    // posição das letras
    Vector2 Position_letters;
    Position_letters.x = chat_box.x + 40;
    Position_letters.y = chat_box.y + 10;

    bool apertado = false;  // variavel booleana que verifica se esta apertado ou nao
    bool digitado = false;  // variavel booleana que verifica se ja foi digitado ou nao

    SetTargetFPS(60); // setando o fps
    InitWindow(screenWitdh, screenHeight, "tela teste"); // inicializiando a janela

    float running_time = 0.0f;  // tempo percorrendo (segundos)
    float update_time = 0.05f;  // tempo de cada frame
    float x = Position_letters.x;   // posição das letras

    while (!WindowShouldClose())    // enquanto a janela estiver aberta
    {
        BeginDrawing();        // começando a desenhar
            ClearBackground(WHITE); // setando o background em branco
        
            if (apertado != true)   // se nao foi apertado
            {
                // mostra o botao pra apertar
                if (GuiButton(botao, "DIALOGO"))
                {
                    // se apertou, ta apertado
                    apertado = true;
                }
            }

            // situação onde verificamos se o botão foi apertadp
            pre_dialogo(&apertado, &digitado, chat_box, frase, Position_letters, &x, &running_time, update_time);
            
            // saida 
            EndDrawing();
        }

    return 0;
}

// função antes do dialogo
void pre_dialogo(bool* apertado, bool* digitado, Rectangle chat_box, char* frase, Vector2 Position_letters, float* x, float* running_time, float update_time)
{
    // se foi apertado:
    if (*apertado == true)
    {
        // desenha a caixa de dialogo
        DrawRectangle(chat_box.x, chat_box.y, chat_box.width, chat_box.height, BLACK);
        
        // se o indice da frase nao chegou ao frim da frase
        if (indice_frase != (int) (strlen(frase) - 1) || *digitado == false)
        {
            // chama a função do dialogo
            dialogo(frase, strlen(frase), *x, Position_letters.y, running_time, update_time, chat_box.width, Position_letters.x);
            // se chegou ao fim..
            if (indice_frase == (int) (strlen(frase) - 1))
            {
                // .. e o usuario apertou o botão "A"
                if (IsKeyPressed(KEY_A) == true)
                {
                    // mostra que foi digitado e que volta ao estado inicial, no caso que nao foi apertado
                    *digitado = true;
                    *apertado = false;
                }   
            }
        }
        
        // se foi digitado:
        if (*digitado == true)
        {
            //reseta as variaveis e retorna a main
            indice_frase = 0;
            *x = Position_letters.x;
            *digitado = false;
        }
    }
}

//função do dialogo (mostra a mensagem por caractere)
void dialogo(char* frase, int tamanho_frase_max, float x, int y, float* running_time, float update_time, float max_width_chat_box, float x_inicial)
{
    // pegando o tempo do frame em segundos
    float deltatime = GetFrameTime();

    // colocamos o nosso tempo em segundos, no caso adicionamos a cada segundo
    *running_time += deltatime;

    // CASO ONDE A FRASE JA CHEGOU AO FIM, MAIS ESPECIFICAMENTE DEIXAR PARADO A MENSAGEM NA TELA
    if (indice_frase == tamanho_frase_max - 1)
    {
        // for para mostrar letra pro letra
        for (int i = 0; i < tamanho_frase_max; i++)
        {
            // se no indice i, tem o caractere ' '..
            if (frase[i] == ' ')
            {
                // e que o proximo caractere ao ser colocado ultrapassaria o limite desejado:
                if (x + ((i + 1) * 17) >= (max_width_chat_box - 100))
                {
                    // pulamos de linha
                    x = x_inicial;
                    // calculo para que ao pular de linha o x esteja no começo exato da proxima linha
                    x = (x - (i * 17) - 17);
                    y += 30;
                    continue;
                }
            }
            
            // se estiver maiusculo a letra:
            if (isupper(frase[i]) == true)
            {
                // desenha a letra e a proxima tera um espaçamento maior entre a maiuscula
                DrawText( TextFormat("%c", frase[i]), x + (i * 17), y, 32, RED);   
                x += 3;
                continue;
            }

            // se a letra for 'l', 'i', 'I' ou 'j':
            if (frase[i] == 'l' || frase[i] == 'i' || frase[i] == 'I' || frase[i] == 'j')
            {
                // desenhamos a letra e a proxima tera um espaçamento menor 
                DrawText( TextFormat("%c", frase[i]), x + (i * 17), y, 32, RED);   
                x -= 10; 
            } else {
                // se nao, apenas desenha a letra
                DrawText( TextFormat("%c", frase[i]), x + (i * 17), y, 32, RED);    
            }
        }
        return;

    } else {   // CASO ONDE A FRASE AINDA NÃO CHEGOU AO FIM, E POR ISSO ESTA AINDA EM SEU PERCUSSO

        // for para mostrar letra pro letra
        for (int i = 0; i <= indice_frase; i++)
        {
            // se no indice i, tem o caractere ' '..
            if (frase[i] == ' ')
            {
                // e que o proximo caractere ao ser colocado ultrapassaria o limite desejado:
                if (x + ((i + 1) * 17) >= (max_width_chat_box - 100))
                {
                    // pulamos de linha
                    x = x_inicial;
                    // calculo para que ao pular de linha o x esteja no começo exato da proxima linha
                    x = (x - (i * 17) - 17);
                    y += 30;
                    continue;
                }
            }
            
            // se estiver maiusculo a letra:
            if (isupper(frase[i]) == true)
            {
                // desenha a letra e a proxima tera um espaçamento maior entre a maiuscula
                DrawText( TextFormat("%c", frase[i]), x + (i * 17), y, 32, RED);   
                x += 3;
                continue;
            }

            // se a letra for 'l', 'i', 'I' ou 'j':
            if (frase[i] == 'l' || frase[i] == 'i' || frase[i] == 'I' || frase[i] == 'j')
            {
                // desenhamos a letra e a proxima tera um espaçamento menor 
                DrawText( TextFormat("%c", frase[i]), x + (i * 17), y, 32, RED);   
                x -= 10; 
            } else {
                // se nao, apenas desenha a letra
                DrawText( TextFormat("%c", frase[i]), x + (i * 17), y, 32, RED);    
            }
        }

        // se o tempo ultrapassou o limite de tempo
        if (*running_time >= update_time)
        {
            // volta pro zero
            *running_time = 0.0f;
            // e vai para o proximo indice da frase
            indice_frase++;
        }
    }
}