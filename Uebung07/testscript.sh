#!/bin/bash

rm Verzeichnisbaum
make

EXEC="./Verzeichnisbaum"
if [[ ! -x "$EXEC" ]]; then
  echo "Fehler: $EXEC existiert nicht oder nicht ausfuehrbar"
  exit 1
fi

echo "==> Starte Testlaufe fuer readDir"
echo

# Hilfsfunktion: Trenner
sep() {
  echo "---------------------------------------------"
}

# Arbeitsverzeichnis vorbereiten
TESTDIR="test_readDir_env"
rm -rf "$TESTDIR"
mkdir "$TESTDIR"

# ----------------------------------------------------
sep
echo "TEST 0: Invalid Call (kein Argument)(2 Args)"
sep
$EXEC
echo
$EXEC "Arg1" "Arg2"
echo

# ----------------------------------------------------
sep
echo "TEST 1: Standardverhalten - aktuelles Verzeichnis"
sep
$EXEC "."
echo

# ----------------------------------------------------
sep
echo "TEST 2: Leeres Verzeichnis"
sep
mkdir "$TESTDIR/empty"
$EXEC "$TESTDIR/empty"
echo

# ----------------------------------------------------
sep
echo "TEST 3: Verzeichnis mit Dateien und Unterverzeichnissen"
sep
mkdir "$TESTDIR/mixed"
touch "$TESTDIR/mixed/a.txt"
touch "$TESTDIR/mixed/b.bin"
mkdir "$TESTDIR/mixed/subdir"
$EXEC "$TESTDIR/mixed"
echo

# ----------------------------------------------------
sep
echo "TEST 4: Nicht-existierendes Verzeichnis"
sep
$EXEC "$TESTDIR/does_not_exist"
echo

# ----------------------------------------------------
sep
echo "TEST 5: Pfad zeigt auf Datei statt Verzeichnis"
sep
echo "Hallo Welt" >"$TESTDIR/file.txt"
$EXEC "$TESTDIR/file.txt"
echo

# ----------------------------------------------------
sep
echo "TEST 6: Verzeichnis ohne Leserechte (r-Bit fehlt)"
sep
mkdir "$TESTDIR/noread"
touch "$TESTDIR/noread/x"
chmod a-r "$TESTDIR/noread"

$EXEC "$TESTDIR/noread"

chmod u+r "$TESTDIR/noread"
echo

# ----------------------------------------------------
sep
echo "TEST 7: Verzeichnis ohne Execute-Rechte (x-Bit fehlt)"
sep
mkdir "$TESTDIR/nosearch"
touch "$TESTDIR/nosearch/z"
chmod a-x "$TESTDIR/nosearch"

$EXEC "$TESTDIR/nosearch"

# Rechte wiederherstellen
chmod u+x "$TESTDIR/nosearch"
echo

# ----------------------------------------------------
sep
echo "TEST 8: Sehr langer Pfad "
sep

# Hole PATH_MAX vom System (typisch 4096, aber wir fragen lieber)
PATH_LIMIT=$(getconf PATH_MAX . 2>/dev/null)
if [[ -z "$PATH_LIMIT" ]]; then
  PATH_LIMIT=4096 # fallback, falls getconf scheitert
fi

LONGBASE="$TESTDIR/longpath"
rm -rf "$LONGBASE"
mkdir -p "$LONGBASE"

cur="$LONGBASE"
segment="subdir_xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

while :; do
  new="$cur/$segment"
  if ((${#new} >= PATH_LIMIT + 10)); then
    break
  fi
  mkdir -p "$new" || break
  cur="$new"
done

echo "Laenge des erzeugten Pfads: ${#cur} (PATH_MAX ~ $PATH_LIMIT)"
echo "Aufruf: $EXEC \"$cur\""
$EXEC "$cur"
echo

# ----------------------------------------------------
sep
echo "TESTS FERTIG"
sep

exit 0
