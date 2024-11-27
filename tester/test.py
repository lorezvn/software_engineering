from init import requests, api_methods, ports
import time

import socket
import random 

def colored_output(text, color_code):
    return f"\033[{color_code}m{text}\033[0m"

def colored_output_bold(text, color_code):
    return f"\033[1;{color_code}m{text}\033[0m"

def key_to_elem(d, elem):
    return next((k for k, v in d.items() if elem in v), None)

class Tester:
    def __init__(self, totale: int, richieste: int, random_seed: int = 10, host: str = "127.0.0.1", debug: bool = False, method_name: str = None):
        """
        Inizializza un'istanza di Tester.
        :param totale: Numero di cicli di test
        :param richieste: Numero di richieste per ciclo
        :param random_seed: Semenza per la generazione casuale
        :param host: Indirizzo IP del server
        :param debug: Modalità di debug
        :param method_name: Metodo da utilizzare (opzionale)
        """
        self.random_seed = random_seed
        self.host = host
        self.debug = debug
        self.debug_method = method_name
        self.totale = totale
        self.richieste = richieste
        self.tot_richieste = self.totale * self.richieste
        self.successful, self.failed = 0, 0
        self.errate = []

        random.seed(self.random_seed)

    def generate_request(self, client):
        """
        Genera una nuova richiesta da inviare
        """
        method_name = self.debug_method if self.debug else random.choice(api_methods[client])

        request_args = []
        for arg_set in requests[method_name]:
            arg_values = {}
            for arg_name, arg_class in arg_set:
                    arg_values[arg_name] = arg_class().generate()
            request_args.append(arg_values)

        request_string = f"{method_name}"
        for arg_values in request_args:
            for arg_name, arg_value in arg_values.items():
                if type(arg_value) == str:
                    arg_value = arg_value.replace(" ", "##")              
                request_string += f" {arg_name} {arg_value}"
        
        return request_string.strip()

    def send_custom_request(self, request_string):
        """
        Invio di una richiesta personalizzata già formattata.
        :param request_string: La richiesta completa da inviare al server
        """
        req_type = request_string.split(" ")[0]
        client = key_to_elem(api_methods, req_type)
        PORT = ports[client]
        
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((self.host, PORT))
            colored_req_type = colored_output(req_type, 33)

            print(f"Inviando la richiesta: {colored_req_type} {request_string[len(req_type):]}")

            sock.send(request_string.encode())
            print(" Richiesta inviata")

            response = sock.recv(2048).decode()

            if response.startswith("BAD_REQUEST") or response.startswith("DB_ERROR"):
                self.failed += 1
                self.errate.append(request_string)
                print(f"{colored_output_bold('[ERRORE]', 31)} Risposta ricevuta: {response}\n")
            else:
                self.successful += 1
                print(f"{colored_output_bold('[SUCCESSO]', 32)} Risposta ricevuta: {response}\n")
                
        self.print_results()

    def send_requests(self):
        """
        Invia richieste multiple
        """
        for i in range(self.totale):
            client = key_to_elem(api_methods, self.debug_method) if self.debug else random.choice(list(api_methods.keys()))
            PORT = ports[client]

            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.connect((self.host, PORT))

                for j in range(self.richieste):
                    current_request = i * self.richieste + j + 1
                    request_string = self.generate_request(client)

                    req_type = request_string.split(" ")[0]
                    colored_req_type = colored_output(req_type, 33)

                    status = f"[{current_request}/{self.tot_richieste}]"
                    print(f"{colored_output_bold(status, 37)} Inviando la richiesta: {colored_req_type} {request_string[len(req_type):]}")

                    sock.send(request_string.encode())
                    print(" Richiesta inviata")

                    response = sock.recv(2048).decode()

                    if response.startswith("BAD_REQUEST") or response.startswith("DB_ERROR"):
                        self.failed += 1
                        self.errate.append(request_string)
                        print(f"{colored_output_bold('[ERRORE]', 31)} Risposta ricevuta: {response}\n")
                    else:
                        self.successful += 1
                        print(f"{colored_output_bold('[SUCCESSO]', 32)} Risposta ricevuta: {response}\n")

                    time.sleep(0.5)

        self.print_results()

    def print_results(self):
        print(f"{colored_output_bold('Testing terminato', 37)}")
        print(f"Richieste soddisfatte: {colored_output_bold(self.successful, 32)}/{self.tot_richieste}")
        print(f"Richieste fallite: {colored_output_bold(self.failed, 31)}/{self.tot_richieste}\n\n")

if __name__ == "__main__":

    #test_for_all()

    tester = Tester(1, 1).send_custom_request("add-prestito richiesta_id 6")
    