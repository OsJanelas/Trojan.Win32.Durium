#include <windows.h>
#include <time.h>
#include <math.h>
#include <vector>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

// --- Vari�veis e Estruturas Globais ---
static ULONGLONG n, r;
int randy() {
    return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff;
}

struct HSL {
    float h, s, l;
};

// COLOR FUNCTIONS
HSL RGBtoHSL(COLORREF rgb) {
    float r = GetRValue(rgb) / 255.0f;
    float g = GetGValue(rgb) / 255.0f;
    float b = GetBValue(rgb) / 255.0f;
    float max = fmaxf(r, fmaxf(g, b)), min = fminf(r, fminf(g, b));
    float h, s, l = (max + min) / 2.0f;
    if (max == min) h = s = 0;
    else {
        float d = max - min;
        s = l > 0.5f ? d / (2.0f - max - min) : d / (max + min);
        if (max == r) h = (g - b) / d + (g < b ? 6 : 0);
        else if (max == g) h = (r - b) / d + 2;
        else h = (r - g) / d + 4;
        h /= 6.0f;
    }
    return { h, s, l };
}

float HueToRGB(float p, float q, float t) {
    if (t < 0.0f) t += 1.0f;
    if (t > 1.0f) t -= 1.0f;
    if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f) return q;
    if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

COLORREF HSLtoRGB(HSL hsl) {
    float r, g, b;
    if (hsl.s == 0) r = g = b = hsl.l;
    else {
        float q = hsl.l < 0.5f ? hsl.l * (1.0f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
        float p = 2.0f * hsl.l - q;
        r = HueToRGB(p, q, hsl.h + 1.0f / 3.0f);
        g = HueToRGB(p, q, hsl.h);
        b = HueToRGB(p, q, hsl.h - 1.0f / 3.0f);
    }
    return RGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
}

// GDI PAYLOADS

// 1. Deslocamento Vertical (Imediato)
DWORD WINAPI payloadVertical(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 12, rand() % 12, w, h, hdc, rand() % 12, rand() % 12, SRCCOPY);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}
    

// 2. T�nel Horizontal (Imediato)
DWORD WINAPI payloadHorizontal(LPVOID lpParam) {
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);

    int ws = w / 8;
    int hs = h / 8;

    BLENDFUNCTION blur = { 0 };
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = ws;
    bmpi.bmiHeader.biHeight = hs;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;

    bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);

    //Set our blur
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 20;

    //Counter variable
    INT i = 0;

    while (1)
    {
        //Random position at the memory
        StretchBlt(dcCopy, rand() % 1, rand() % 1, ws, hs, dc, rand() % -1, rand() % -1, w, h, SRCCOPY);

        for (int x = 0; x < ws; x++)
        {
            for (int y = 0; y < hs; y++)
            {
                int index = y * ws + x;

                rgbquad[index].rgbRed += i;
                rgbquad[index].rgbGreen += i;
                rgbquad[index].rgbBlue += i;
            }
        }

        i++;

        Sleep(rand() % 500);
        AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, blur);
    }

    return 0x00;
}

// 3. GDI Mistos (Espera 8s)
DWORD WINAPI payloadGDI(LPVOID lpParam) {
    Sleep(8000);
    // Esconde a janela do console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Frases do seu código
    const char* lpStrings[] = {
        "Do pills work?", "I don't want live anymore", "Waiting for my end",
        "Everyday is a infinite loop", "My thoughts are killing me", "Kill me",
        "Dissociating", "Fleeing reality", "Am I Real?", "I'am not safe",
        "HELP ME", "cold.", "Transcending", "Anxiety", "Hear the voices",
        "Trapped by illusion", "Full of emptiness", "Dreaming forever",
        "Questioning my existence", "Losing control regressively", "pain",
        "Don't want to suffer anymore", "Panic attacks", "Dark forever",
        "Faking my happiness", "Seeking eternal peace", "Can't breathe", "sad",
        "Existential Crisis", "Viewing shadows", "Beyond the stars", "Alone",
        "Can't kill myself", "What am I", "Made with rotten flesh", "Isolated",
        "Wearing masks", "My final destination", "Hurting myself", "Farewell"
    };

    int stringCount = sizeof(lpStrings) / sizeof(lpStrings[0]);

    while (true) {
        HDC hdc = GetDC(NULL);

        // Configuração da Fonte (baseado no seu LOGFONT)
        LOGFONT lFont = { 0 };
        lFont.lfWidth = 20;
        lFont.lfHeight = 50;
        lFont.lfOrientation = rand() % 3600; // Rotação aleatória para caos visual
        lFont.lfEscapement = lFont.lfOrientation;
        lFont.lfWeight = 800;
        lFont.lfUnderline = TRUE;
        lFont.lfQuality = DRAFT_QUALITY;
        lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
        lstrcpy(lFont.lfFaceName, L"Arial");

        HFONT hFont = CreateFontIndirect(&lFont);
        SelectObject(hdc, hFont);

        // Define cores aleatórias (estilo "glitch")
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkMode(hdc, TRANSPARENT);

        // Lógica de sorteio (rand() % 25 == 24 como no seu snippet)
        if (rand() % 25 == 24) {
            int x = rand() % w;
            int y = rand() % h;
            const char* selectedText = lpStrings[rand() % stringCount];

            // Desenha o texto na tela
            TextOutA(hdc, x, y, selectedText, strlen(selectedText));
        }

        // Limpeza para não vazar memória
        DeleteObject(hFont);
        ReleaseDC(NULL, hdc);

        // Sleep curto para não travar o processador, mas manter o efeito rápido
        Sleep(10);
    }

    return 0;
}

// 4. Distor��o RGB (Espera 20s)
DWORD WINAPI payloadRGB(LPVOID lpParam) {
    Sleep(20000);
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Lista de ícones do sistema para alternar
    LPCWSTR icons[] = { IDI_APPLICATION, IDI_HAND, IDI_QUESTION, IDI_EXCLAMATION, IDI_ASTERISK, IDI_WINLOGO, IDI_SHIELD };

    double angle = 0;
    int radius = 100;

    while (true) {
        HDC hdc = GetDC(NULL);

        // Pega a posição atual do mouse para o centro do círculo
        POINT cursor;
        GetCursorPos(&cursor);

        // Calcula a posição X e Y no círculo usando seno e cosseno
        // Usamos LaTeX para a lógica: $x = \cos(\theta) \cdot r$ e $y = \sin(\theta) \cdot r$
        int x = cursor.x + (int)(cos(angle) * radius);
        int y = cursor.y + (int)(sin(angle) * radius);

        // Seleciona um ícone aleatório da lista
        HICON hIcon = LoadIcon(NULL, icons[rand() % 7]);

        // Desenha o ícone na posição calculada
        DrawIcon(hdc, x, y, hIcon);

        // Incrementa o ângulo para o próximo ícone girar
        angle += 0.5;

        // Faz o raio expandir e contrair levemente para um efeito pulsante
        radius = 100 + (int)(sin(angle * 0.1) * 50);

        // Libera o DC
        ReleaseDC(NULL, hdc);

        // Delay para controlar a velocidade da "cobra" de ícones
        Sleep(10);

        // Chance de limpar a tela ocasionalmente
        if (rand() % 100 == 99) {
            InvalidateRect(NULL, NULL, TRUE);
        }
    }

    return 0;
}

// 5. Rota��o de Hue HSL (Espera 40s para efeito final)
DWORD WINAPI payloadHSL(LPVOID lpParam) {
    Sleep(40000);
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);

        // Aplica o BitBlt com a ROP customizada 0x123456
        BitBlt(hdc, rand() % 5, rand() % 5, sw, sh, hdcMem, rand() % 5, rand() % 5, 0x123456);

        DeleteObject(brush);
        DeleteObject(bm);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

int main() {
    srand(time(NULL));
    // ShowWindow(GetConsoleWindow(), SW_HIDE);
    CreateThread(NULL, 0, payloadVertical, NULL, 0, NULL);
    CreateThread(NULL, 0, payloadHorizontal, NULL, 0, NULL);
    CreateThread(NULL, 0, payloadGDI, NULL, 0, NULL);
    CreateThread(NULL, 0, payloadRGB, NULL, 0, NULL);
    CreateThread(NULL, 0, payloadHSL, NULL, 0, NULL);
    while (1) Sleep(1000);
    return 0;
}