from init import requests, api_methods

import socket
import random 
import string

HOST = "127.0.0.1"
RANDOM_SEED = 10

ports = {"bibliotecario": 42069, "fornitore": 42070, "utente": 42071}

def generate_request(client):
    method_name = random.choice(api_methods[client])

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
                arg_value.replace(" ", "##")              
            request_string += f" {arg_name} {arg_value}"
    
    return request_string.strip()

if __name__ == "__main__":
    totale = 50
    richieste = 5
    succesful = 0
    failed = 0
    errate = []
    
    for _ in range(totale):
        
        client = random.choice(list(api_methods.keys()))
        PORT = ports[client]
        
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
  
            for _ in range(richieste):
                request_string = generate_request(client)
                print(f"Inviando la richiesta: {request_string}")
    
                s.send(request_string.encode()) # Invia la richiesta al server
                print("post invio")
                
                response = s.recv(2048).decode() # Ricevi la risposta dal server
                print(f"Risposta ricevuta: {response}")
    
                if response.startswith("BAD_REQUEST") or response.startswith("DB_ERROR"):
                    failed += 1
                    errate.append(request_string)
                else:
                    succesful += 1
                            
    print(f"\n succesful requests: {succesful}/{totale*richieste} \n failed requests: {failed}/{totale*richieste} \n\n")