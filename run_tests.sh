#!/bin/sh

nom_interpreteur=curiosity-test$1

# Vérifie si le fichier 'curiosity-test' existe, sinon le compile
if [ ! -f ./$nom_interpreteur ];
then
    echo "Programme de test non trouvé, compilation de <curiosity-test>"
    make $nom_interpreteur
fi

tests_passed=0;
tests_total=0;

echo 
echo ============== TESTS ==============
# Parcourt chaque fichier dans le répertoire 'tests' et exécute 'curiosity-test' avec le fichier en paramètre
for test in tests/*.test; do
    tests_total=$(($tests_total+1))
    ./$nom_interpreteur -q "$test"
    if [ $? -eq 0 ];
    then 
        tests_passed=$(($tests_passed+1))
    fi
done
echo ===================================
echo $tests_passed/$tests_total passed, $(($tests_total-$tests_passed)) failed
echo ===================================
echo

if [ $tests_passed -eq $tests_total ]; then
    exit 0;
fi

exit 1;
