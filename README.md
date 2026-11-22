# follow

logiciel de comparaison de deux textes

## Prérequis

- `make`, `git`, `gcc`, `pkg-config`, `asan`/`ubsan`
- GTK 3 (bibliothèques de développement)

> Si vous êtes sur une distro redHat/Fedora, vous aurez besoin d'installer `asan` et `ubsan` pour exécuter le programme avec le *sanitizer*, ainsi que les paquets de développement GTK 3 :
>
> ```shell
> sudo dnf install asan ubsan gtk3-devel pkg-config
> ```
>
> Sur Debian/Ubuntu, installez les paquets de développement GTK 3 :
>
> ```shell
> sudo apt-get install libgtk-3-dev pkg-config
> ```

## Guide d'utilisation

### Initialisation

Tout d'abord, veuillez cloner le dépôt si ce n'est pas déjà fait :

```shell
git clone https://github.com/RugiSerl/follow
```

Ensuite exécutez la commande :

```shell
make setup
```

Cette tâche s'occupera de télécharger la dépendance Unity pour les tests.

### Exécution

Pour exécuter le projet, faites simplement :

```shell
make
```

Cela va compiler et exécuter le programme.

### Tests

Pour lancer le jeu de test, faites :

```shell
make test
```

> ⚠️ Attention : si vous n'avez pas fait `make setup` (qui installe Unity) vous allez avoir des erreurs !