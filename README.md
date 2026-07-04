# Tiger Compiler - Compilateur du langage Tiger en C++

Ce projet est un compilateur pour le langage **Tiger** (issu du livre *Modern Compiler Implementation* d'Andrew Appel), réalisé en **C++20** dans le cadre du projet d'assistants YAKA à EPITA. Le binaire `tc` couvre toute la chaîne de compilation, du fichier source jusqu'à la génération de code **LLVM IR**.

## Fonctionnalités

- **Analyse lexicale et syntaxique** (Re/Flex + Bison) transformant le source Tiger en arbre de syntaxe abstraite (AST).
- **Affichage de l'AST** via un pretty-printer et un export au format DOT.
- **Analyse sémantique** : liaison des noms (binding), gestion des portées et vérification de types statique.
- **Extensions du langage** : Tiger objet (méthodes, classes) et surcharge de fonctions (overloading).
- **Transformations** : désucrage (desugar) et analyse des échappements (escape analysis) des variables.
- **Optimisations** : graphe d'appels, inlining et combinaison.
- **Génération de code** : traduction vers LLVM IR avec support du ramasse-miettes (garbage collector).

## Technologies

- C++20 (libc++)
- Re/Flex (≥ 3.2.10)
- Bison (≥ 3.8.2)
- LLVM 18 (`clang`, `llvm-config`, `llvm-link`)
- GNU Autotools (Autoconf / Automake)
- Nix (flake)
- Boost

## Installation

### Prérequis

- Un compilateur C++ supportant C++20 (Clang recommandé)
- LLVM 18 (`llvm-config-18`, `clang-18`, `llvm-link-18`)
- Re/Flex (≥ 3.2.10) et Bison (≥ 3.8.2)
- Les outils GNU Autotools (`autoconf`, `automake`, `libtool`)
- Boost

Un environnement de développement reproductible est également fourni via Nix (`nix develop`).

### Compilation

1. Cloner le projet

```bash
git clone git@github.com:YashLeBg/tiger-compiler.git
cd tiger-compiler
```

2. Générer le système de build

```bash
./bootstrap
```

3. Configurer le projet

```bash
./configure
```

4. Compiler

```bash
make
```

### Exécution

Le binaire produit est `tc`. Il prend en entrée un fichier source Tiger :

```bash
./src/tc fichier.tig
```

Utilisez `./src/tc --help` pour la liste complète des options disponibles.

### Tests

```bash
make check
```

## Contributeurs

Projet réalisé dans le cadre du cursus ING1 à EPITA (promotion 2028).

- [Anli-Yach Mohamed](https://github.com/YashLeBg)
- [Louis-Maximilien Chappuit](https://github.com/louis3117)
- [Colin Gomez](https://github.com/Aznred)
- [Jessim Ziani](https://github.com/jessim-ziani)
