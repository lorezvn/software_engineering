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
- [x] add-richiesta-prestito 
- [x] ricerca-libri (da ricontrollare)
- [ ] visualizza-prestiti-attivi 
- [ ] visualizza-cronologia
- [x] visualizza-sanzioni (da ricontrollare)
- [x] update-username


### Bibliotecario
- [x] add-bibliotecario
- [ ] visualizza-catalogo (?)
- [ ] visualizza-copie
- [ ] add-sanzione
- [ ] estingui-sanzione
- [ ] revoca-sanzione
- [ ] add-prestito
- [ ] update-prestito
- [ ] visualizza-richieste-prestito
- [ ] update-richiesta-prestito
- [ ] add-richiesta-restock


### Fornitore
- [x] add-fornitore
- [ ] add-restock
- [ ] visualizza-richieste-restock
- [ ] update-richiesta-restock


## TO-DO
- [ ] Sistemare relazione
- [ ] Sistemare UML 
- [ ] Aggiornare Trigger
- [ ] Sistemare monitor non funzionali
- [ ] Sistemare Tester (alcuni test dovrebbero essere fatti meglio ad esempio per coppie di date inizio/fine)

