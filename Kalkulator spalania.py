import random
import os

# ---------------------------------------------
# Funkcja sprawdzająca i przygotowująca plik CSV
# ---------------------------------------------
"""
Użytkownik może wybrać nazwę pliku.
Funkcja .strip() sprawia że zostają usunięte wszelkie
niepotrzebne spacje w nazwie pliku. 
Do wybranej nazwy zostaje dodane rozszerzenie .csv
"""

def przygotuj_plik():
    nazwa = input("Podaj nazwę pliku (bez rozszerzenia, Enter = Dane_spalania): ").strip()
    if not nazwa:
        nazwa = "Dane_spalania"
    nazwa_pliku = f"{nazwa}.csv"

    """
    Jeśli plik istnieje, program zapyta, czy go nadpisać.
    Tworzy nagłówek CSV, jeśli plik jest nowy.
    """

    if os.path.exists(nazwa_pliku):
        print("---------------------------------------")
        decyzja = input(f"Plik '{nazwa_pliku}' istnieje. Czy chcesz go nadpisać? (T/N): ").strip().upper()
        if decyzja == "T":
            os.remove(nazwa_pliku)
            print("Plik został usunięty i utworzony na nowo.")
        else:
            print("Dane zostaną dopisane do istniejącego pliku.")
    else:
        print(f"Nowy plik '{nazwa_pliku}' zostanie utworzony.")

    # Dodaje nagłówek, jeśli plik jest pusty
    if not os.path.exists(nazwa_pliku) or os.stat(nazwa_pliku).st_size == 0:
        with open(nazwa_pliku, "a", encoding="utf-8-sig") as file:
            file.write("Marka_samochodu;Srednie_spalanie[l/100km];Kilometry;Koszt_Lpg[zł];Koszt_Benzyny[zł];Koszt_Diesla;Wlane_litry\n")

    return nazwa_pliku




# ---------------------------------------------
# Funkcja obliczająca koszty transportu
# ---------------------------------------------

"""
Funkcja oblicza średnie spalanie i koszt paliwa dla danej podróży.
W trybie testowym dane generowane są losowo (Aby łatwiej można było wykryć potencjalne błędy w kodzie).
"""
def ile_wydane_na_transport( tryb_testowy=False):
    # Najczęstsze marki aut w Polsce (Kolejność jest losowa)
    najpopularniejsze_firmy_w_polsce = [
        "Toyota", "Volkswagen", "Skoda", "Ford", "Opel", "Renault", "Dacia",
        "Hyundai", "Kia", "Peugeot", "Fiat", "BMW", "Mercedes-Benz",
        "Audi", "Nissan", "Mazda", "Seat", "Citroen", "Honda", "Suzuki"
    ]
    lista_wag = []  # wagi pasażerów
    # ---------------------------------------------
    # TRYB TESTOWY — Dane są generowane losowo
    # ---------------------------------------------

    if tryb_testowy:
        print("TRYB TESTOWY WŁĄCZONY")
        marka = random.choice(najpopularniejsze_firmy_w_polsce)
        kilometry = random.uniform(50,750)
        wlane_litry = random.uniform(10,70)
        cena_benzyny = random.uniform(6,8)
        cena_lpg = random.uniform(2,4)
        cena_diesel = random.uniform(4.75,6.5)
        ile_osob = random.randint(1,4)
        lista_wag = [random.uniform(55,95) for _ in range(ile_osob)]
    else:
        # ---------------------------------------------
        # TRYB NORMALNY — dane zależne od użytkownika
        # ---------------------------------------------
        marka = input("Podaj markę samochodu: ").capitalize()
        while marka not in najpopularniejsze_firmy_w_polsce:
            print("Niepoprawna nazwa firmy. Spróbuj ponownie.")
            marka = input("Podaj markę samochodu: ").capitalize()

        # Zdobywanie i sprawdzanie danych liczbowych
        while True:
            try:
                kilometry = float(input("Ile kilometrów przejechano?: "))
                wlane_litry = float(input("Ile wlano litrów paliwa?: "))
                break
            except ValueError:
                print("Wprowadź poprawne wartości liczbowe.")

        # Ilość pasażerów i ich wagi.
        # Duża ilość pasażerów może negatywnie
        # wpłynąć na spalanie auta.
        ile_osob = int(input("Ile osób w aucie? (0 jeśli nie chcesz uwzględniać): "))
        for x in range(ile_osob):
            while True:
                try:
                    waga = float(input(f"Waga osoby {x + 1} (kg): "))
                    lista_wag.append(waga)
                    break
                except ValueError:
                    print("Wprowadź poprawną liczbę.")

        # Zdobywanie od użytkownika rodzaju paliwa oraz jego ceny za litr.
        decyzja = ""
        while decyzja != "LPG" and decyzja != "BEN" and decyzja != "DI":
            decyzja = input("Jakie paliwo? (LPG/BEN/DI): ").upper()
            if decyzja == "LPG":
                cena_lpg = float(input("Cena LPG (zł/l): "))
                cena_benzyny = 0
                cena_diesel = 0
            elif decyzja == "BEN":
                cena_benzyny = float(input("Cena benzyny (zł/l): "))
                cena_lpg = 0
                cena_diesel = 0
            elif decyzja == "DI":
                cena_diesel = float(input("Cena diesla (zł/l): "))
                cena_lpg = 0
                cena_benzyny = 0
            else:
                print("Niepoprawny typ paliwa. Podaj typ jeszcze raz (LPG/BEN/DI): ")
                decyzja = input("Jakie paliwo? (LPG/BEN/DI): ").upper()

    # ---------------------------------------------
    # OBLICZENIA
    # ---------------------------------------------
    
    srednie_spalanie = (wlane_litry / kilometry) * 100
    '''
    Uwzględnienie masy pasażerów (0.6% spalania na każde 100kg średniej wagi)
    '''
    if len(lista_wag) > 0:
        spalanie_dodatkowe = sum(lista_wag) * 0.004 
        print(f"OTO JEST SPALANIE DODATKOWE: {spalanie_dodatkowe}")
        srednie_spalanie += spalanie_dodatkowe

    # Koszty dla benzyny, LPG i diesla
    koszt_benzyny = round((srednie_spalanie * kilometry / 100 * cena_benzyny), 2)
    koszt_lpg = round((srednie_spalanie * kilometry / 100 * cena_lpg), 2)
    koszt_diesla = round((srednie_spalanie * kilometry / 100 * cena_diesel), 2)
    srednie_spalanie = round(srednie_spalanie, 3)

    # ------------------------------------------------
    # Zmienna wynik zwraca dane do zapisu w pliku .csv
    # ------------------------------------------------
    wynik = f"{marka};{srednie_spalanie};{round(kilometry,1)};{koszt_lpg};{koszt_benzyny};{koszt_diesla};{round(wlane_litry,2)}\n"
    wynik = wynik.replace(".",",") # - bez tej linii kodu wartości były źle odczytywane przez Excela
    return wynik


# ---------------------------------------------
# Funkcja główna — obsługa wielu wywołań
# ---------------------------------------------
def wielokrotne_wywolanie():
    """
    Pozwala wykonać funkcję ile_wydane_na_transport() wiele razy.
    Dane są zapisywane do wybranego pliku CSV.
    """
    print("=== OBLICZANIE KOSZTÓW TRANSPORTU ===")
    print("---------------------------------------")

    # Przygotowanie pliku, w którym
    # będą zapisywane dane
    nazwa_pliku = przygotuj_plik()

    # Ile razy wykonać obliczenia
    while True:
        try:
            liczba = int(input("Ile razy chcesz wykonać obliczenia?: "))
            if liczba > 0:
                break
            print("Podaj liczbę większą od 0.")
        except ValueError:
            print("Wprowadź poprawną liczbę.")

    tryb_testowy = input("Czy włączyć tryb testowy? (T/N): ").strip().upper() == "T"

    # ---------------------------------------------
    # Główna pętla obliczeń i zapisu
    # ---------------------------------------------
    for i in range(liczba):
        wynik = ile_wydane_na_transport(tryb_testowy)
        with open(nazwa_pliku, "a", encoding="utf-8-sig") as file:
            file.write(wynik)
        print(f"Zapisano wpis {i+1} do pliku {nazwa_pliku}")
        print("---------------------------------------")

    print("---------------------------------------")
    print(f"Zakończono. Dane dostępne w pliku: {nazwa_pliku}")
    print("---------------------------------------")

    # Otwórz plik po zakończeniu (opcjonalnie)
    otworz = input("Czy chcesz otworzyć plik? (T/N): ").strip().upper()
    if otworz == "T":
        try:
            os.startfile(nazwa_pliku)
        except Exception:
            print("Nie udało się otworzyć pliku automatycznie.")


# ---------------------------------------------
# Punkt startowy programu
# ---------------------------------------------
if __name__ == "__main__":
    wielokrotne_wywolanie()
