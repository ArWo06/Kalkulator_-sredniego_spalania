# include <stdio.h>
# include <stdbool.h>
#include <stdlib.h>
# include <math.h>
# include <time.h>
# include <string.h>
#include <ctype.h>

typedef struct {
    char marka[50];
    char model[50];
    double srednie_spalanie_na_100km;
    double kilometry;
    double koszt_lpg;
    double koszt_benzyny;
    double koszt_diesla;
    double wlane_litry;
}dane_do_excela;

void koszty_transportu(bool tryb_testowy);
void przygotowanie_pliku(dane_do_excela dane);

int main(){

    printf("Kalkulator Spalania \n");
    srand(time(NULL));
    koszty_transportu(false); 


    return 0;
}

void koszty_transportu(bool tryb_testowy)
{
    char marka[30] = "";
    char model[30] = "";
    double kilometry = 0.0f;
    double wlane_litry = 0.0f;
    double cena_benzyny = 0.0f;
    double cena_lpg = 0.0f;
    double cena_diesla = 0.0f;
    int ile_osob = 0;
    double lista_wag[5] = {0};
    char rodzaj_paliwa = ' ';
    if (tryb_testowy)
    {   //-----------------------------
        //LOSOWANIE PODSTAWOWYCH DANYCH 
        //-----------------------------
        strcpy(marka, "Marka");
        strcpy(model, "Testowy");
        int min_kilometry = 50;
        int max_kilometry = 500;
        kilometry = (rand() % (max_kilometry - min_kilometry + 1)) + min_kilometry;

        int min_wlane_litry = 15;
        int max_wlane_litry = 80;
        wlane_litry = (rand() % (max_wlane_litry - min_wlane_litry + 1)) + min_wlane_litry;

        int min_cena_benzyny = 6;
        int max_cena_benzyny = 9;
        cena_benzyny = (rand() % (max_cena_benzyny - min_cena_benzyny + 1)) + min_cena_benzyny;

        int min_cena_lpg = 3;
        int max_cena_lpg = 5;
        cena_lpg = (rand() % (max_cena_lpg - min_cena_lpg + 1)) + min_cena_lpg;

        int min_cena_diesla = 5;
        int max_cena_diesla = 7;
        cena_diesla = (rand() % (max_cena_diesla - min_cena_diesla + 1)) + min_cena_diesla;

        int min_ile_osob = 1;
        int max_ile_osob = 5;
        ile_osob = (rand() % (max_ile_osob - min_ile_osob + 1)) + min_ile_osob;

        int min_waga = 55;
        int max_waga = 95;

        for (int i = 0; i < ile_osob; i++)
        {
            lista_wag[i] = (rand() % (max_waga - min_waga + 1)) + min_waga;
        }

        //WYŚWIETLENIE DANYCH DO TESTÓW
        /*
        printf("Kilometry: %.2lf\n", kilometry);
        printf("Wlane litry: %.2lf\n", wlane_litry);
        printf("Cena benzyny: %.2lf\n", cena_benzyny);
        printf("Cena LPG: %.2lf\n", cena_lpg);
        printf("Cena diesla: %.2lf\n", cena_diesla);
        printf("Liczba osob: %d\n", ile_osob);
        
        for (int i = 0; i < ile_osob; i++)
        {
            printf("Waga osoby %d: %d kg\n", i + 1, lista_wag[i]);
        } */
    }
    else{
        // ------------------------------
        // Użytkownik sam wpisuje dane
        // ------------------------------
        do {
            printf("Podaj marke auta: ");
            scanf(" %29s",marka);
        }while (strlen(marka) <1);

        do {
            printf("Podaj model auta: ");
            scanf(" %29s",model);
        }while (strlen(model) <1);

        do{
            printf("Podaj przejechane kilometry: ");
            scanf(" %lf",&kilometry);
        }while(kilometry <1);

        do{
            printf("Podaj wlane litry: ");
            scanf(" %lf",&wlane_litry);
        }while(wlane_litry <1);

        do{
            printf("Podaj liczbe osob (0-5): ");
            scanf(" %d",&ile_osob);
        }while(ile_osob <0 || ile_osob >5);

        for (int i = 0; i < ile_osob; i++)
        { do {
            printf("Podaj wage osoby nr.%d (10kg - 150kg): ", i+1);
            scanf("%lf",&lista_wag[i]);} while(lista_wag[i] < 10 || lista_wag[i] > 150);
        }

        printf("Podaj rodzaj paliwa: (B/L/D)");
        scanf(" %c",&rodzaj_paliwa);
        rodzaj_paliwa = toupper(rodzaj_paliwa);
        
        switch (rodzaj_paliwa)
        {
        case 'B':
            do{
            printf("Podaj cene benzyny: ");
            scanf("%lf",&cena_benzyny);
            }while(cena_benzyny <1);
            break;

        case 'L':
            do{
            printf("Podaj cene lpg: ");
            scanf("%lf",&cena_lpg);
            }while(cena_lpg <1);
            break;

        case 'D':
            do{
            printf("Podaj cene diesla: ");
            scanf("%lf",&cena_diesla);
            }while(cena_diesla <1);
            break;

        default:
            printf("PODANO NIEPOPRAWNY TYP PALIWA!!!");
            printf("PROGRAM PODA NIEPOPRAWNE WARTOSCI");
            break;
        }  /*
        printf("Kilometry: %lf\n", kilometry);
        printf("Wlane litry: %lf\n", wlane_litry);
        printf("Cena benzyny: %lf\n", cena_benzyny);
        printf("Cena LPG: %lf\n", cena_lpg);
        printf("Cena diesla: %lf\n", cena_diesla);
        printf("Liczba osob: %d\n", ile_osob);
        

        for (int i = 0; i < ile_osob; i++)
        {
            printf("Waga osoby %d: %lf kg\n", i + 1, lista_wag[i]);
        }
        */
    }
    // -----------------------------------------
    // OBLICZENIA
    // -----------------------------------------

    double srednie_spalanie_na_100km = (wlane_litry / kilometry) * 100;

    // Uwzględniamy masę pasażerów (0.6% spalania na każde 100kg wagi)
    if ((ile_osob) > 0){
        double spalanie_dodatkowe = 0;
        double laczna_waga_pasazerow = 0;
        for(int i = 0; i < ile_osob ;i++){
            laczna_waga_pasazerow+=lista_wag[i];
        }
        spalanie_dodatkowe = (laczna_waga_pasazerow/100) * 0.6;
        srednie_spalanie_na_100km +=spalanie_dodatkowe;
    }

   cena_benzyny = srednie_spalanie_na_100km * kilometry / 100 * cena_benzyny;
   cena_lpg = srednie_spalanie_na_100km * kilometry/ 100*cena_lpg ;
   cena_diesla = srednie_spalanie_na_100km * kilometry / 100 * cena_diesla;

    //--------------------------
    // PRZYGOTOWYWANIE STRUKTURY
    //--------------------------
    dane_do_excela dane;
    strcpy(dane.marka, marka);
    strcpy(dane.model, model);
    dane.kilometry = kilometry;
    dane.wlane_litry = wlane_litry;
    dane.srednie_spalanie_na_100km = srednie_spalanie_na_100km;
    dane.koszt_benzyny = cena_benzyny;
    dane.koszt_lpg = cena_lpg;
    dane.koszt_diesla = cena_diesla;

    przygotowanie_pliku(dane);
    printf("Dane zostaly zapisane! \n"); 
}

//-----------------------------
// FUNKCJA ZAPISUJĄCA PLIK CSV
//-----------------------------

void przygotowanie_pliku(dane_do_excela dane){
    int czy_domyslna_nazwa = 0;
    int czy_nadpisac_plik = 0;
    char nazwa_pliku[50] = "dane_spalania";
    do{printf("Czy chcesz zmieniac nazwe pliku? (Domyslnie: dane_spalania)(1 - NIE / 2 - TAK): \n");
    scanf("%d",&czy_domyslna_nazwa);
    } while(czy_domyslna_nazwa != 1 && czy_domyslna_nazwa != 2);
    
    if(czy_domyslna_nazwa == 2){
        do{
            printf("Podaj nazwe pliku: \n");
            scanf("%30s",nazwa_pliku);
    }while(strlen(nazwa_pliku) <3);
    }

    strcat(nazwa_pliku,".csv");
    printf("Tak wyglada teraz nazwa pliku: %s\n",nazwa_pliku);

    do{printf("Czy chcesz nadpisac dane w pliku? (1 - NIE / 2 - TAK): \n");
    scanf("%d",&czy_nadpisac_plik);
    }while(czy_nadpisac_plik !=1 && czy_nadpisac_plik !=2);


    FILE * pPlik = NULL;
    if(czy_nadpisac_plik==1){
        pPlik = fopen(nazwa_pliku,"a");
    }
    else if (czy_nadpisac_plik==2)
    {
        pPlik = fopen(nazwa_pliku,"w");
    } 

    if(!pPlik){
        printf("Nie można otworzyć pliku %s\n",nazwa_pliku);
        return;
    }
    
    

    // Jeżeli plik jest pusty dodaje nagłówek

    fseek(pPlik, 0, SEEK_END);
    if (ftell(pPlik) == 0){
        fprintf(pPlik, "Marka;Model;Srednie_spalanie;Kilometry;Koszt_LPG;Koszt_Benzyny;Koszt_Diesla;Wlane_litry\n");
    }

    fprintf(pPlik, "%s;%s;%.2lf;%.2lf;%.2lf;%.2lf;%.2lf;%.2lf\n",
        dane.marka, dane.model, dane.srednie_spalanie_na_100km, dane.kilometry,
        dane.koszt_lpg, dane.koszt_benzyny, dane.koszt_diesla, dane.wlane_litry);

    fclose(pPlik);
}