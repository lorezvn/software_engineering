# Software Engineering Homework Project

Project realized for 'Software Engineering' exam.

- Paolo Marchetti (1986485)
- Davide Vittucci (1903954)
- Lorenzo Zanda (2006432)

## Installation
### Steps for installation:

Clone the repository:
```sh
git clone https://github.com/lorezvn/software_engineering.git
cd software_engineering
```

Run the `install.sh` script to install the required dependencies:
```sh
sh install.sh
```
Run the `compile.sh` script to build the project:
```sh
sh compile.sh
```

## Running the project

To start the project, run the following command:
```sh
sh run.sh
```

To stop all processes related to the project, run:
```sh
sh run.sh --kill
```

## Api Methods


### Utente
- add-utente
- add-richiesta-prestito
- ricerca-libri
- visualizza-prestiti 
- visualizza-sanzioni 
- update-username


### Bibliotecario
- add-bibliotecario
- add-edizione
- add-sanzione 
- add-prestito
- add-richiesta-restock
- visualizza-copie 
- estingui-sanzione
- revoca-sanzione
- termina-prestito
- visualizza-richieste-prestito
- visualizza-restock
- rifiuta-richiesta-prestito
- delete-copia


### Fornitore
- add-fornitore
- add-restock
- visualizza-richieste-restock
- rifiuta-richiesta-restock


## TO-DO
- [x] Per operazioni di visualizzazione da "stack smashed" -> Cercare su readFromFunctions in `server/handler.cpp` la causa
- [x] Sistemare Tester (alcuni test dovrebbero essere fatti meglio ad esempio per coppie di date inizio/fine)
- [x] Nel caso in cui ci siano richieste con argomenti spaziati il sistema non lo gestisce correttamente (add-richiesta-restock da rivedere)
- [x] Aggiornare trigger 
    * [x]  Fare in modo che i dati di restock/prestito siano uguali alle richieste
    * [x]  Uno stesso libro non possa essere prestato contemporaneamente
    * [x]  I trigger col cf sono da modificare
    * [x]  Per le richieste non dev'essere possibile rifiutare se la richiesta non è "IN ATTESA"
    * [x]  Le sanzioni possono essere possono essere estinte solo se sono "NON PAGATE"
    * [x]  Le sanzioni possono essere revocate solo se "NON PAGATE"
- [ ] Sistemare relazione
- [x] Sistemare UML 
- [x] Sistemare monitor non funzionali
- [x] Rivedere meglio la gestione di dataInizio e fine in RichiestaPrestito (andrebbero cancellate e lasciate solo in Prestito)
- [x] Le richieste prestito dovrebbero avere l'edizione di riferimento e non il libro fisico (se alcune copie non sono disponibili se ne cercano altre)
- [x] Rivedere autori e generi dei libri (per adesso non sono considerati)
- [x] Modificare vincoli molteplicità generedizione e genereautore in maniera tale da averne massimo un genere e massimo un autore
- [ ] Rivedere situazioni in cui qualche attributo è NULL (eseguire ricerca-libri)
- [ ] Sistemare qualche generatore per i test
