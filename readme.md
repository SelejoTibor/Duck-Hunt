# Duck Hunting Game  

Ez a projekt egy egyszerű "kacsa vadászat" játékot valósít meg, amelyben a játékosnak gyorsan kell reagálnia, hogy minél több kacsát eltaláljon.

---

## Játék Információk  

### A játék célja  
A játék során a képernyőn véletlenszerűen megjelenő kacsákat kell lelőni. A kacsák:  
- Az **alsó alfanumerikus kijelző** valamelyik négy mezőjének **legfelső vízszintes szegmensében** jelennek meg (zöld szegmens).  
- A kacsák a nehézségi szinttől függően csak rövid ideig maradnak láthatóak.  
- A vadásznak az **alsó vízszintes szegmenssel** alájuk kell állnia és lelőnie őket.  

---

## Játék Indítása  

1. **Nehézségi szint beállítása:**  
   - A játék indulása előtt a nehézségi szintet a **kapacitív csúszka** segítségével állíthatja be.  
   - A kijelzőn megjelenik az aktuális nehézségi szint.  

2. **Játék indítása:**  
   - A nehézségi szint beállítása után nyomja meg a **PB1** gombot a játék elindításához.  

---

## Játék Menete  

- **Vadászat:**  
  - A vadászt a **kapacitív csúszka** segítségével mozgathatja a megfelelő pozícióba.  
  - Nyomja meg a **PB0** gombot a lövéshez.  

- **Lövés megjelenítése:**  
  - A lövés a **függőleges szegmensen** (sárga szegmens) látható.  
  - Ha eltalál egy kacsát, az villogással (villogó kacsa szegmens) jelzi a találatot.  

- **Játék vége:**  
  - Összesen 25 kacsa levadászására van lehetőség.  
  - A játék véget ér, ha ez a szám elérte a maximumot.  

---

## Eredmények Megjelenítése  

- Az **eddig megjelent kacsák** és az **eltalált kacsák száma** a felső numerikus kijelzőn látható.  

---

## Technikai Követelmények  

- **Hardver:**  
  - EFM32 Giant Gecko  
