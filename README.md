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

## Test the project
After the project is running (see [Running the project](#running-the-project)) to test the API methods, follow these steps:
1. Navigate to the ```tester``` directory:
```sh
cd tester
```
2. Run the test script:
```sh
python test.py
```
This will initiate testing for all the API methods listed in the section [API Methods](#api-methods).

### Customizing the Test Behavior
You can modify the number of requests made by the ```test_for_all()``` function by adjusting the following field in the code:
*   richieste: Specifies the number of requests per API method.

## API Methods

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

