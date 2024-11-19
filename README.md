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
- [x] add-utente
- [x] add-richiesta-prestito (da ricontrollare)
- [x] ricerca-libri (da ricontrollare)
- [x] visualizza-prestiti 
- [x] visualizza-sanzioni (da ricontrollare)
- [x] update-username


### Bibliotecario
- [x] add-bibliotecario
- [x] add-sanzione (guardare TODO)
- [x] add-prestito
- [x] add-richiesta-restock
- [ ] visualizza-copie
- [x] estingui-sanzione
- [x] revoca-sanzione
- [ ] termina-prestito
- [ ] visualizza-richieste-prestito
- [ ] rifiuta-richiesta-prestito


### Fornitore
- [x] add-fornitore
- [ ] add-restock
- [ ] visualizza-richieste-restock
- [ ] rifiuta-richiesta-restock


## TO-DO
- [x] Per operazioni di visualizzazione da "stack smashed" -> Cercare su readFromFunctions in `server/handler.cpp` la causa
- [ ] Sistemare relazione
- [ ] Sistemare UML 
- [ ] Aggiornare Trigger
- [ ] Sistemare monitor non funzionali
- [x] Sistemare Tester (alcuni test dovrebbero essere fatti meglio ad esempio per coppie di date inizio/fine)
- [ ] Rivedere meglio la gestione di dataInizio e fine in RichiestaPrestito (andrebbero cancellate e lasciate solo in Prestito)
- [ ] Le richieste prestito dovrebbero avere l'edizione di riferimento e non il libro fisico (se alcune copie non sono disponibili se ne cercano altre)
- [ ] Nel caso in cui ci siano richieste con argomenti spaziati il sistema non lo gestisce correttamente (add-richiesta-restock da rivedere)
