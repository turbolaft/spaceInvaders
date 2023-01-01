# SPACE INVADERS
> to launch the game U need to write 'make' in ur terminal and then open it by typing './game'
### The game is written using SDL2 on linux ubuntu, to launch U need to install all SDL libraries,

Program se začina v filu main.c, Nejdříve inicializujeme struct app, kde máme náš renderer a window, a proměnný left, right, fire kteři určený pro činů z hračem.

Takže v structs.h je definované  Entity player je naš hrač

V common.h jsou definované skoro všechny textures jako globalně proměnny, aby zvětšit performance během cyklu

V initSDL() (init.c) vytvořime renderer a window, Dal uložime všechny textures, a to je pro performance, abychom netravili moc čásu v cyklě, je ten čás realně duležitý pro nás
funkce loadTextures() a loadSurface jsou v draw.c

##Linked List
Inicializujeme všechný proměnný pro player bullet, enemies, enemyBullets, walls, extraEnemya pro effekt z underLine

Přidame hodnoty hraču x, y, width, height, a tak dal.... a začiname nekonečný cykl

##Cykl
V cyklu máme dvě nejduležitejši funkce prepareScene() a presentScene v (draw.h), 
presentScene je pro mal'ovaní všechno co máme v renderer, prepareScene() pro čištění rendereru

doInput() v input.c pro přečtení všech keyboard events pro hrača a východ z hrý když klikneme ne křížek

handlePlayer() v handlingPlayer.c nam umožní posunout hrača, a vystřelit, pro kulky použivame linked list, abychom mohli střilet hned několik krat i když předchozí bullet má bullet->health = 1

handlingEnemy() v enemies.c, nam umožní hnout enemies, měnit jich obrazék a střilet z randomného enemy pomocí rand() % (numberOfEnemiesAlive + 1), 

drawBullets() v handlingPlayer.c nam vykreslí všechny kulky(a přida nové souřadnice pronich) a ověři jestli jich  souřadnice dosahují souřadnice enemies, nebo extraEnemy, a naopak pokud je to kulka od enemy tak ověři jestli dosahuje hrača, takže je funkce drawBullets(), je pro enemy bullet tak i pro player bullet


drawingUnderLine() v underLine.c nam vykresli přimku dolu, a vykresli intervaly kteři byli zničiny od enemy bullet, tyto intervaly jsou uloženy v underLineTearIntervals linked listu

blit(player.texture, player.x, player.y, player.width, player.height), vykresli nam hráča, funkce blit je definována v draw.c a umožni nam uložit texture v renderer pomocí SDL_RenderCopy

spawnExtraEnemy() v enemies.c je pro vykreslení extraEnemy, jestli extraEnemy->tick == EXTRA_ENEMY_APPEARANCES(je určeno v defs.h kde máme hodně konfiguraci) 

Dál počítani score a vykreslení pomocí funkce pro pracé s textem downloadingText() (v handlingPlayer.c) 

drawingWalls() v walls.c nam ulože v renderer stěny z novým surface(to se může měnit, když enemy kulka zasáhne stěnu, tak bude nějaký výbuch)

wallBoom() v walls.c nam překresli zničený pixely v černu barvu, zničený pixely udělame rekurzivně

showGameOverScene() na koncí nam vypíše score board, a sčita predchozí scores z filu a dopíše nový, limit je 10