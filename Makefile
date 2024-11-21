CC=clang -Wall

PROGRAMMES=test_terrain test_robot robot_terrain curiosity curiosity-test test_generation_terrains curiosity-perf curiosity-obs

all: $(PROGRAMMES)

######################################################################
#                       Règles de compilation                        #
######################################################################

%.o: %.c
	$(CC) -c $<

test_terrain.o: test_terrain.c terrain.h

test_robot.o: test_robot.c robot.h

robot_terrain.o: robot_terrain.c terrain.h robot.h

robot.o: robot.c robot.h

terrain.o: terrain.c terrain.h

environnement.o: environnement.c environnement.h robot.h terrain.h

programme.o: programme.c programme.h type_pile.h

interprete.o: interprete.c interprete.h environnement.h \
	programme.h type_pile.h robot.h terrain.h

interprete%.o: interprete%.c interprete.h environnement.h \
	programme.h type_pile.h robot.h terrain.h

type_pile.o: type_pile.c type_pile.h

curiosity.o: curiosity.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

curiosity-test.o: curiosity-test.c environnement.h programme.h \
	interprete.h robot.h terrain.h type_pile.h

generation_terrains.o: generation_terrains.h generation_terrains.c

test_generation_terrains.o: test_generation_terrains.c generation_terrains.h terrain.h

curiosity-perf.o: curiosity-perf.c terrain.h environnement.h programme.h interprete.h

observateur.o: observateur.h observateur.c

curiosity-obs.o: curiosity-obs.c programme.h environnement.h interprete.h terrain.h robot.h

######################################################################
#                       Règles d'édition de liens                    #
######################################################################

test_terrain: test_terrain.o terrain.o
	$(CC) $^ -o $@

test_robot: test_robot.o robot.o
	$(CC) $^ -o $@

robot_terrain: robot_terrain.o terrain.o robot.o
	$(CC) $^ -o $@

curiosity: curiosity.o environnement.o observateur.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

curiosity-test: curiosity-test.o environnement.o observateur.o programme.o interprete.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

curiosity-test%: curiosity-test.o environnement.o observateur.o programme.o interprete%.o \
	robot.o terrain.o type_pile.o
	$(CC) $^ -o $@

test_generation_terrains: test_generation_terrains.c generation_terrains.o terrain.o
	$(CC) $^ -o $@

curiosity-perf: curiosity-perf.o robot.o generation_terrains.o terrain.o environnement.o observateur.o programme.o interprete.o type_pile.o

curiosity-obs: curiosity-obs.o environnement.o programme.o observateur.o interprete.o terrain.o robot.o type_pile.o

clean:
	rm -f $(PROGRAMMES) curiosity-test0 curiosity-test1 curiosity-test2 curiosity-test3 curiosity-test4 curiosity-test5 curiosity-test6 curiosity-test7 curiosity-test8 curiosity-test9 *.o
