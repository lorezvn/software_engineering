from init import requests, api_methods
import time

import socket
import random 
import string

HOST = "127.0.0.1"
RANDOM_SEED = 10

ports = {"bibliotecario": 42069, "fornitore": 42070, "utente": 42071}

DEBUG = True
debug = {"method_name": "add-sanzione", "client": "bibliotecario"}

def generate_request(client):
    method_name = debug["method_name"] if DEBUG else random.choice(api_methods[client])

    request_args = []
    for arg_set in requests[method_name]:
        arg_values = {}
        for arg_name, arg_class in arg_set:
            if isinstance(arg_name, tuple) and arg_class.__name__ == "DateGenerator":
                start_date = arg_name[0]
                end_date = arg_name[1]
                arg_values[start_date], arg_values[end_date] = arg_class().generate_start_end()
            else:
                arg_values[arg_name] = arg_class().generate()
        request_args.append(arg_values)

    request_string = f"{method_name}"
    for arg_values in request_args:
        for arg_name, arg_value in arg_values.items():
            if type(arg_value) == str:
                arg_value = arg_value.replace(" ", "##")              
            request_string += f" {arg_name} {arg_value}"
    
    return request_string.strip()


def colored_output(text, color_code):
    return f"\033[{color_code}m{text}\033[0m"

def colored_output_bold(text, color_code):
    return f"\033[1;{color_code}m{text}\033[0m"

# Main
if __name__ == "__main__":
    totale = 1
    richieste = 5
    tot_richieste = totale * richieste
    successful, failed = 0, 0
    errate = []

    print()

    for i in range(totale):
        client = debug["client"] if DEBUG else random.choice(list(api_methods.keys()))
        PORT = ports[client]

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))

            for j in range(richieste):
                current_request = i * richieste + j + 1
                request_string = generate_request(client)

                status = f"[{current_request}/{tot_richieste}]" 
                print(f"{colored_output_bold(status, 37)} Inviando la richiesta: {request_string}")

                s.send(request_string.encode())
                print(" Richiesta inviata")

                response = s.recv(2048).decode()

                if response.startswith("BAD_REQUEST") or response.startswith("DB_ERROR"):
                    failed += 1
                    errate.append(request_string)
                    print(f"{colored_output_bold('[ERRORE]', 31)} Risposta Ricevuta: {response}\n") 
                else:
                    successful += 1
                    print(f"{colored_output_bold('[SUCCESSO]', 32)} Risposta Ricevuta: {response}\n") 

                time.sleep(0.5) 

    # Risultati finali
    print(f"Richieste soddisfatte: {colored_output_bold(successful, 32)}/{tot_richieste}")
    print(f"Richieste fallite: {colored_output_bold(failed, 31)}/{tot_richieste}\n")