# ChatRoomOS-2021-2022

## Code Ereur

*NORMAL_EXIT*(0) : Code de retour de programme qui s'est terminé avec succès.

*EXIT_BAD_USAGE_ERROR*(1) : Mauvaises utilisations d'arguments.

*EXIT_SOCK_ERROR*(2) : Une erreur qui survient lors d'une connection ou communication avec le serveur.

*EXIT_OTHER_ERROR*(3) : Tout autre type d'erreur qui surviendrait lors de l'exécution du programme.


## Lancer le salon de chat 

### Prérequis 
Étant donné deux exécutables, client et server, lancez le server.
Par la suite, lancez les clients avec les bons paramètres.


### Server 
Le serveur se lance via la commande suivante :
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

Dans le terminal se déplacer jusque dans le dossier source du MakeFile.
```shell
source make
```

Par la suite, vous pourrez trouver les exécutables dans le répertoire ```./bin/```.
