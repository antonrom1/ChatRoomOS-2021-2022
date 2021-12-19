# ChatRoomOS-2021-2022

## Code Ereur

*NORMAL_EXIT*(0) : Le code de retour d'une fin de programme avec succès.

*EXIT_BAD_USAGE_ERROR*(1): L'utilisateur n'entre pas les bons arguments pour se connecter.

*EXIT_SOCK_ERROR*(2): Une erreur qui survient lors d'une connection au server. 

*EXIT_OTHER_ERROR*(3): Tout autre type d'erreur qui surviendrait lors de l'exécution de l'app.


## Lancer le salon de chat 

### Prérequis 
Etant donner une application server-client vous devez commencer par lancer le server.
Dans la suite lancer les clients avec les bons paramètres de connexion server.


### Server 
Le serevr se lance via la commande 
```shell 
source ./bin/server <port>
```


### Client 
Pour chacun des clients lancer la commande 
```shell 
source ./bin/client <pseudo> <ip_serveur> <port>
```

## Terminaison

### Client 
"ctrl+D" pour se déconnecter du server.
Un cas de terminaison est la perte de connection avec le server.

### Server
"ctrl+C" pour arrêter le server. Ce signal est capturé de manière à déconneceter les clients après l'arrêt du server.

## Compilation locale

Dans le terminale se déplacer jusque dans le dossier source du MakeFile.
```shell
source make
```
