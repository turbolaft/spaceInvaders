# SPACE INVADERS
> to launch the game U need to write 'make' in ur terminal and then open it by typing './game'
### The game is written using SDL2 on linux ubuntu, to launch U need to install all SDL libraries,

Program začiná souborem **main.c**, nejdříve inicializuje **struct:app**, kde je náš **renderer** a **window**, a proměnné: **left**, **right**, **fire** které, jsou  určené pro činnost z hráče.

V **structs.h** je definovaná entita player, toto je náš hráč.

V **common.h** jsou definované skoro všechny textury jako globalní proměnné, aby zvyšovaly rychlost běhu programu během cyklu.

V **initSDL()** (./init.c) vytvořime **renderer** a **window**, dále uložíme všechny textury, a to kvůli zvýšení rychlosti běhu programu, abychom netravili příliš mnoho čásu v cyklu.

funkce **loadTextures()** a **loadSurface()** jsou v **draw.c**

##Linked List
Inicializujeme všechny proměnné pro player **bullet**, **enemies**, **enemyBullets**, **walls**, **extraEnemy** a pro effekt z **underLine**

Přidáme hodnoty hráčů **x**, **y**, **width**, **height**, a tak dále... a začínáme nekonečnou smyčku

##Cyklus
V cyklu máme dvě nejduležitejši funkce: **prepareScene()** a **presentScene()** v (draw.h), 
**presentScene()** je pro výpis všeho, co máme v rendereru, **prepareScene()** pro čištění rendereru

**doInput()** v input.c pro přečtení všech keyboard eventů pro hrača, a ukončení hry, když klikneme ne křížek.

**handlePlayer()** v **handlingPlayer.c** nam umožní posunout hrača, a vystřelit, pro kulky použivame linked list, abychom mohli střilet hned několik krát, i když předchozí bullet má **bullet->health = 1**

**handlingEnemy()** v **enemies.c**, nám umožní pohnout nepřátely, měnit jich obrazék a střilet z náhodného nepřítele pomocí: **rand() % (numberOfEnemiesAlive + 1)**, 

**drawBullets()** v **handlingPlayer.c** nam vykreslí všechny kulky(a přida nové souřadnice pro ně), a ověří jestli jejich souřadnice obsahují souřadnice nepřátel, nebo **extraEnemy**, a naopak pokud je to kulka od nepřátele, tak ověři jestli dosahuje hráče, pro toto je funkce **drawBullets()**.


**drawingUnderLine()** v **underLine.c** nám vykresli přímku dolů, a vykreslí intervaly, které byly zničiny od **enemy bullet**, tyto intervaly jsou uloženy v **underLineTearIntervals** linked listu.

**blit(player.texture, player.x, player.y, player.width, player.height)**, nám vykreslí hráče, funkce **blit** je definována v **draw.c** a umožni nám uložit texturu v rendereru pomocí **SDL_RenderCopy**.

**spawnExtraEnemy()** v **enemies.c** je pro vykreslení **extraEnemy**, jestli **extraEnemy->tick == EXTRA_ENEMY_APPEARANCES** (je určeno v defs.h kde máme hodně konfigurací). 

Dál počítání skóre, a vykreslení pomocí funkce pro pracé s textem **downloadingText()** (v handlingPlayer.c).

**drawingWalls()** v **walls.c** nám uloží v rendereru stěny s novým surface (to se může měnit, když enemy kulka zasáhne stěnu, tak bude nějaký výbuch).

**wallBoom()** v **walls.c** nám překreslí zničené pixely, na černu barvu, zničené pixely uděláme rekurzivně.

**showGameOverScene()** na konci nam vypíše score-board, a sečte předchozí skóre ze souboru, a dopíše nové( limit je 10).
