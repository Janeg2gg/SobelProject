#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int Header_Size = 14;
const int Info_Header_Size = 40;

struct BGRColor
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct BM_Header
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfRes1;
    unsigned short bfRes2;
    unsigned int bfOffBits;
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXpelsPerMeter;
    unsigned int biYpelsPerMeter;
    unsigned int biCrlUses;
    unsigned int biCrlImportant;
};

void BM_Data (struct BM_Header &Header,unsigned char File_Header[Header_Size], unsigned char Info_Header[Info_Header_Size])
{
    //--------------DANE DLA File_Header-------------------------
    Header.bfType = File_Header[0] + (File_Header[1] << 8);                                                           //Sygnatura "BM"
    Header.bfSize = File_Header[2] + (File_Header[3] << 8) + (File_Header[4] << 16) + (File_Header[5] << 24);         //Długość całego pliku
    Header.bfRes1 = File_Header[6] + (File_Header[7] << 8);                                                           //Pole zarezerwowane
    Header.bfRes2 = File_Header[8] + (File_Header[9] << 8);                                                           //Pole zarezerwowane
    Header.bfOffBits = File_Header[10] + (File_Header[11] << 8) + (File_Header[12] << 16) + (File_Header[13] << 24);  //Pozycja danych obrazowych w pliku

    //--------------DANE DLA INFO_HEADER-------------------------
    Header.biSize = Info_Header[0] + (Info_Header[1] << 8) + (Info_Header[2] << 16) + (Info_Header[3] << 24);                 //Rozmiar nagłówka
    Header.biWidth = Info_Header[4] + (Info_Header[5] << 8) + (Info_Header[6] << 16) + (Info_Header[7] << 24);                //Szerokość obrazu
    Header.biHeight = Info_Header[8] + (Info_Header[9] << 8) + (Info_Header[10] << 16) + (Info_Header[11] << 24);             //Wysokość obrazu
    Header.biPlanes = Info_Header[12] + (Info_Header[13] << 8);                                                               //Liczba płatów
    Header.biBitCount = Info_Header[14] + (Info_Header[15] << 8);                                                             //l.bitow na piskel
    Header.biCompression = Info_Header[16] + (Info_Header[17] << 8) + (Info_Header[18] << 16) + (Info_Header[19] << 24);      //Algorytm kompresji
    Header.biSizeImage = Info_Header[20] + (Info_Header[21] << 8) + (Info_Header[22] << 16) + (Info_Header[23] << 24);        //Rozmiar rysunku
    Header.biXpelsPerMeter = Info_Header[24] + (Info_Header[25] << 8) + (Info_Header[26] << 16) + (Info_Header[27] << 24);    //Rozdzielczosc pozioma
    Header.biYpelsPerMeter = Info_Header[28] + (Info_Header[29] << 8) + (Info_Header[30] << 16) + (Info_Header[31] << 24);    //Rozdzielczosc pionowa
    Header.biCrlUses = Info_Header[32] + (Info_Header[33] << 8) + (Info_Header[34] << 16) + (Info_Header[35] << 24);          //l. kolorow w palecie
    Header.biCrlImportant = Info_Header[36] + (Info_Header[37] << 8) + (Info_Header[38] << 16) + (Info_Header[39] << 24);     //l.waznych koloro w palecie

    cout << "bfType: " << Header.bfType << endl;
    cout << "bfSize: " << Header.bfSize << endl;
    cout << "reserved 1: " << Header.bfRes1 << endl;
    cout << "reserved 2: " << Header.bfRes2 << endl;
    cout << "bfOffBits: " << Header.bfOffBits << endl;
    cout << "biSize: " << Header.biSize << endl;
    cout << "biWidth: " << Header.biWidth << endl;
    cout << "biHeight: " << Header.biHeight << endl;
    cout << "biPlanes: " << Header.biPlanes << endl;
    cout << "biBitCount: " << Header.biBitCount << endl;
    cout << "biCompression: " << Header.biCompression << endl;
    cout << "biSizeImage: " << Header.biSizeImage << endl;
    cout << "biXpelsPerMeter: " << Header.biXpelsPerMeter << endl;
    cout << "biYpelsPerMeter: " << Header.biYpelsPerMeter << endl;
    cout << "biCrlUses: " << Header.biCrlUses << endl;
    cout << "biCrlImportant: " << Header.biCrlImportant << endl;

    system("PAUSE");
}

void BM_Write(struct BM_Header &Header,vector<BGRColor> &bi_Color,unsigned char File_Header[Header_Size], unsigned char Info_Header[Info_Header_Size])
{
    string file_name_out ;
    cout << "Podaj nazwe pliku BMP do zapisu: " << endl;
    cin >> file_name_out ;

    fstream plik;
    plik.open(file_name_out, ios::out | ios::binary);

    if(!plik.is_open())
    {
        cout << "Plik nie zostal utworzony" << endl;
        return;
    }

    unsigned char BM_padding[3] = {0, 0, 0};                        //bit paddingu
    const int Padding = ((4 - (Header.biWidth* 3) % 4 ) % 4);       //ilosc bitow padding'u

    plik.write(reinterpret_cast<char*>(File_Header), (Header_Size));
    plik.write(reinterpret_cast<char*>(Info_Header), (Info_Header_Size));

    for(int y = 0; y < Header.biHeight; y++)
    {
        for(int x = 0; x < Header.biWidth; x++)
        { 
            
            unsigned char r = static_cast<unsigned char>(bi_Color[y * Header.biWidth + x].R );
            unsigned char g = static_cast<unsigned char>(bi_Color[y * Header.biWidth + x].G );
            unsigned char b = static_cast<unsigned char>(bi_Color[y * Header.biWidth + x].B );
            
            unsigned char color[] = {b, g, r};
            plik.write(reinterpret_cast<char*>(color), 3);
            
        }

        plik.write(reinterpret_cast<char*>(BM_padding), Padding);
    }
    plik.close();

    cout << "Plik zostal stworzony" << endl;
}




int main(){

    string file_name ;
    cout << "Podaj nazwe pliku BMP do odczytu: " << endl;
    cin >> file_name ;

    //otworzenie pliku
    ifstream plik;
    plik.open(file_name, ios::in | ios::binary);

    if(!plik.is_open())
    {
        cout << "Nie mozna otworzyc pliku" << endl;
        return 0;
    }

    unsigned char File_Header[Header_Size];
    plik.read(reinterpret_cast<char*>(File_Header), Header_Size);

    //sprawdzenie czy plik jest Bitmapa
    if(File_Header[0] != 'B' || File_Header[1] != 'M')
    {
        cout << "To nie jest plik .bmp" << endl;
        plik.close();
        return 0;
    }

    unsigned char Info_Header[Info_Header_Size];
    plik.read(reinterpret_cast<char*>(Info_Header), Info_Header_Size);

    BM_Header Header;
    BM_Data(Header,File_Header, Info_Header); //dane z naglowka

    //--------------przypisanie pikseli rgb -------------------
    vector<BGRColor> bi_Color;
    bi_Color.resize(Header.biHeight * Header.biWidth);
    const int Padding = ((4 - (Header.biWidth* 3) % 4 ) % 4);

    for(int y = 0; y < Header.biHeight; y++)
    {
        for(int x = 0; x < Header.biWidth; x++)
        {
            unsigned char color[3];
            plik.read(reinterpret_cast<char*>(color), 3);
            bi_Color[y * Header.biWidth + x].R = (color[2]) ; 
            bi_Color[y * Header.biWidth + x].G = (color[1]) ; 
            bi_Color[y * Header.biWidth + x].B = (color[0]) ; 
        }

        plik.ignore(Padding);
    }
    plik.close();
    //---------------------------------------------------------

    //-----------------algorytm sobela----------------------
    int Gx[3][3] = {{-1, 0 , 1},            //macierze przez ktore bedziemy mnozyc
                    {-2, 0 , 2}, 
                    {-1, 0 , 1} };
    int Gy[3][3] = {{-1, -2, -1}, 
                    { 0, 0 , 0}, 
                    { 1, 2 , 1} };

    int pixel_x, pixel_y;
    vector<BGRColor> bi_Color2;
    bi_Color2.resize(Header.biHeight * Header.biWidth);
    int pixel_xr, pixel_xg, pixel_xb, pixel_yr, pixel_yg, pixel_yb;     //potrzebne zmienne do obliczenia dla rgb

     for (int x=1; x < Header.biWidth-1; x++)
    {
        for (int y=1; y < Header.biHeight-1; y++)
        {
            pixel_xr = (Gx[0][0] * bi_Color[Header.biWidth * (y-1) + (x-1)].R)
                    + (Gx[0][1] * bi_Color[Header.biWidth * (y-1) + x ].R)
                    + (Gx[0][2] * bi_Color[Header.biWidth * (y-1) + (x+1)].R)
                    + (Gx[1][0] * bi_Color[Header.biWidth * y + (x-1)].R)
                    + (Gx[1][1] * bi_Color[Header.biWidth * y + x ].R)
                    + (Gx[1][2] * bi_Color[Header.biWidth * y + (x+1)].R)
                    + (Gx[2][0] * bi_Color[Header.biWidth * (y+1) + (x-1)].R)
                    + (Gx[2][1] * bi_Color[Header.biWidth * (y+1) + x ].R)
                    + (Gx[2][2] * bi_Color[Header.biWidth * (y+1) + (x+1)].R);

            pixel_yr = (Gy[0][0] * bi_Color[Header.biWidth * (y-1) + (x-1)].R)
                    + (Gy[0][1] * bi_Color[Header.biWidth * (y-1) + x ].R)
                    + (Gy[0][2] * bi_Color[Header.biWidth * (y-1) + (x+1)].R)
                    + (Gy[1][0] * bi_Color[Header.biWidth * y + (x-1)].R)
                    + (Gy[1][1] * bi_Color[Header.biWidth * y + x ].R)
                    + (Gy[1][2] * bi_Color[Header.biWidth * y + (x+1)].R)
                    + (Gy[2][0] * bi_Color[Header.biWidth * (y+1) + (x-1)].R)
                    + (Gy[2][1] * bi_Color[Header.biWidth * (y+1) + x ].R)
                    + (Gy[2][2] * bi_Color[Header.biWidth * (y+1) + (x+1)].R);

            pixel_xg = (Gx[0][0] * bi_Color[Header.biWidth * (y-1) + (x-1)].G)
                    + (Gx[0][1] * bi_Color[Header.biWidth * (y-1) + x ].G)
                    + (Gx[0][2] * bi_Color[Header.biWidth * (y-1) + (x+1)].G)
                    + (Gx[1][0] * bi_Color[Header.biWidth * y + (x-1)].G)
                    + (Gx[1][1] * bi_Color[Header.biWidth * y + x ].G)
                    + (Gx[1][2] * bi_Color[Header.biWidth * y + (x+1)].G)
                    + (Gx[2][0] * bi_Color[Header.biWidth * (y+1) + (x-1)].G)
                    + (Gx[2][1] * bi_Color[Header.biWidth * (y+1) + x ].G)
                    + (Gx[2][2] * bi_Color[Header.biWidth * (y+1) + (x+1)].G);

            pixel_yg = (Gy[0][0] * bi_Color[Header.biWidth * (y-1) + (x-1)].G)
                    + (Gy[0][1] * bi_Color[Header.biWidth * (y-1) + x ].G)
                    + (Gy[0][2] * bi_Color[Header.biWidth * (y-1) + (x+1)].G)
                    + (Gy[1][0] * bi_Color[Header.biWidth * y + (x-1)].G)
                    + (Gy[1][1] * bi_Color[Header.biWidth * y + x ].G)
                    + (Gy[1][2] * bi_Color[Header.biWidth * y + (x+1)].G)
                    + (Gy[2][0] * bi_Color[Header.biWidth * (y+1) + (x-1)].G)
                    + (Gy[2][1] * bi_Color[Header.biWidth * (y+1) + x ].G)
                    + (Gy[2][2] * bi_Color[Header.biWidth * (y+1) + (x+1)].G);

            pixel_xb = (Gx[0][0] * bi_Color[Header.biWidth * (y-1) + (x-1)].B)
                    + (Gx[0][1] * bi_Color[Header.biWidth * (y-1) + x ].B)
                    + (Gx[0][2] * bi_Color[Header.biWidth * (y-1) + (x+1)].B)
                    + (Gx[1][0] * bi_Color[Header.biWidth * y + (x-1)].B)
                    + (Gx[1][1] * bi_Color[Header.biWidth * y + x ].B)
                    + (Gx[1][2] * bi_Color[Header.biWidth * y + (x+1)].B)
                    + (Gx[2][0] * bi_Color[Header.biWidth * (y+1) + (x-1)].B)
                    + (Gx[2][1] * bi_Color[Header.biWidth * (y+1) + x ].B)
                    + (Gx[2][2] * bi_Color[Header.biWidth * (y+1) + (x+1)].B);

            pixel_yb = (Gy[0][0] * bi_Color[Header.biWidth * (y-1) + (x-1)].B)
                    + (Gy[0][1] * bi_Color[Header.biWidth * (y-1) + x ].B)
                    + (Gy[0][2] * bi_Color[Header.biWidth * (y-1) + (x+1)].B)
                    + (Gy[1][0] * bi_Color[Header.biWidth * y + (x-1)].B)
                    + (Gy[1][1] * bi_Color[Header.biWidth * y + x ].B)
                    + (Gy[1][2] * bi_Color[Header.biWidth * y + (x+1)].B)
                    + (Gy[2][0] * bi_Color[Header.biWidth * (y+1) + (x-1)].B)
                    + (Gy[2][1] * bi_Color[Header.biWidth * (y+1) + x ].B)
                    + (Gy[2][2] * bi_Color[Header.biWidth * (y+1) + (x+1)].B);

            int valr = (int)sqrt((pixel_xr * pixel_xr) + (pixel_yr * pixel_yr));
            int valg = (int)sqrt((pixel_xg * pixel_xg) + (pixel_yr * pixel_yg));
            int valb = (int)sqrt((pixel_xb * pixel_xb) + (pixel_yb * pixel_yb));

            //wartosci nie moge wykraczac poza <0;255>
            if(valr < 0) valr = 0;
            if(valr > 255) valr = 255;
            if(valg < 0) valg = 0;
            if(valg > 255) valg = 255;
            if(valb < 0) valb = 0;
            if(valb > 255) valb = 255;

            bi_Color2[Header.biWidth * y + x].R = valr;
            bi_Color2[Header.biWidth * y + x].G = valg;
            bi_Color2[Header.biWidth * y + x].B = valb;
        }
    }


    //zapisywanie do pliku
    BM_Write(Header,bi_Color2,File_Header, Info_Header);


    return 0;
}

    //Wzór na padding ((4 - (biWidth* 3) % 4 ) % 4);