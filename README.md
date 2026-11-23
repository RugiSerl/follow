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

### Compilation pour Windows (cross-compilation)

Sur Fedora, installez le toolchain croisé et GTK 3 pour Windows :

```shell
sudo dnf install mingw64-gcc mingw64-gtk3 pkgconf-pkg-config
```

Compilez ensuite l'application :

```shell
make windows
```

L'exécutable est produit dans `bin/windows/follow.exe`. Pour obtenir un dossier
prêt à être utilisé sur Windows (exécutable + DLL GTK/GLib) :

```shell
make windows-dist
```

Le contenu de `bin/windows-dist` est alors à copier sur la machine Windows.

### Tests

Pour lancer le jeu de test, faites :

```shell
make test
```

> ⚠️ Attention : si vous n'avez pas fait `make setup` (qui installe Unity) vous allez avoir des erreurs !