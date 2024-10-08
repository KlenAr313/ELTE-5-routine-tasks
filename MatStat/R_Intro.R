#########################################
###   Bevezetés az R programozásba    ###
#########################################

## R hivatalos oldala ##
# https://cran.r-project.org/

# Szabad és nyílt forrású statisztikai és grafikai 
# programozási nyelv és interaktív környezet.

# Eszköztára klasszikus és modern statisztikai és matematikai
# eljárást tartalmaz az alaprendszer részeként vagy csomagokban.
# Az R a CRAN (Comprehensive R Archive Network) szervereken áll 
# rendelkezésre. Ezek ugyanazt a tartalmat kínáló, földrajzilag 
# a világ különböző részein található szerverek, melyek mindegyikén 
# elérhető a legutóbbi R verzió több platformra, az R telepíto állományok 
# forráskódja és ez szintén több ezer ún. "user contributed" csomag tárolója. 

## R népszerűsége ##
# https://spectrum.ieee.org/top-programming-language-2020
# https://pypl.github.io/PYPL.html
# https://www.tiobe.com/tiobe-index/

## RStudio ##
# Integrált fejlesztői környezet az R-hez:
# https://www.rstudio.com/products/rstudio/

## R Markdown ##
# Egyszerű szintaxist biztosít dokumentumok sima szöveg
# formájában történő írásához, melyből többféle kimenet
# (például HTML, PDF) állítható elő.
# Lehetővé teszi R kódrészek beágyazását a dokumentumokba.
# Sok más nyelv használata is támogatott (például C, SAS, SQL, Python, stb.).
# https://rmarkdown.rstudio.com/lesson-1.html

## R dokumentáció ##
# https://cran.r-project.org/doc/manuals/r-release/R-intro.html
# https://www.rdocumentation.org/


##################################################################################

##########################
## egyszeru számolások: ##
##########################

2+3
4*3 + sqrt(16)
log(2)/9^2

1/0
0/0  # NaN: not a number, nem összekeverendő NA-val, ami a hiányzó adat jele (üres elem)
Inf-Inf

3 < 4
5 < 1
3 == 6/2
1+5*3-1==(1+5)*(3-1)

## objektum orientált           ##
## értékadó operátor: <- vagy = ##

azen.osszegem <- 2+3
azen.osszegem

nevem <- "Bevezetés az R programozásba"
nevem

azen.osszegem; nevem

##################
## adattípusok: ##
##################

## character
nev <- c("en")
nev

is.character(nev)
is.character(3)
is.character("3")

as.character(nev)
as.character(3)

## numerical
3

is.numeric(3)
is.numeric(nev)
is.numeric("3")

as.numeric("3")

## logical
vektor <- c(2, 3, 4)
vektor == 3
logi <- vektor == 3
logi

is.logical(logi)

#####################
## adatstruktúrák: ##
#####################

  ##############
  ### VEKTOR ###
  ##############

c()
c(1,2,3)
c("mi", "ti", "ok")

1:10
seq(from=3, to=6, by=1)
seq(-2,3,0.5)		# -2-től +3-ig 0,5-ösével
rep(3,5)			# 3 szám 5-ször egymás után
rep(c(1,2),2)		# kétszer ismétli meg az 1,2 -t
rep(c(1,2),each=2)	# mindet 2-szer ismétli meg
rep(c(1,2),c(2,4))	# első elemet 2-szer, másodikat 4-szer ismétli meg


rep(4, times = 5)


c(3, "én", TRUE)  # character
c(3, TRUE, FALSE) # numeric
# vektor minden eleme ugyanolyan típusú

vector(length=3)	# üres vektor definiása

v <- c(1,2,4,6,7)
is.vector(v)
length(v)
dim(v) 		# dimenzió - vektornak nincs
str(v)    # struktúra
class(v)  # klasszifikáció

v[3]			# vektor 3. eleme
v[1:3]            # vektor 1-től a 3. elemei
v[-4]             # vektor 4. elemét kivesszük
v[-c(1,3)]        # vektor 1. és 3. elemét kivesszük
v[v>6]            # vektor 5-nél nagyobb elemei (szintén vektor)

d <- c(1,2); e <- c(3,4)
d%*%e      		# skaláris szorzás
t(e)        	# transzponálás
d%*%t(e)	   	# mátrixszorzás

  ##############
  ### MATRIX ###
  ##############
  # kétdimenziós (r×c) objektum; minden eleme ugyanolyan típusú kell legyen

matrix(c(1, 2, 3, 4, 5, 6), nrow = 2, ncol = 3)
matrix(c(1, 2, 3, 4, 5, 6), ncol = 3, byrow = T)
matrix(c(1,2,3,4,5,6,7,8,9,10), nrow = 2)
matrix(1:10, c(2,5))
matrix(1:10, 2)

matrix(nrow = 2, ncol = 3)	# üres matrix definiálása

m <- matrix(c(1,2,4,5,7,8), nrow = 2); m
length(m)
dim(m)
str(m)
class(m)

m[2,1]		# mátrix 2. sorának 1. eleme
m[-2,-1]          # mátrix 2. sorának és 1. oszlopának kihagyása
apply(m,1,sum)    # sum (összegzés) függvény a mátrix soraira
apply(m,2,sum)    # sum (összegzés) függvény a mátrix oszlopaira

  ############
  ### TÖMB ###
  ############
  # többdimenziós objektum; minden eleme ugyanolyan típusú kell legyen

array(0, dim = c(2, 3, 4))
array(1:24, c(2, 3, 2, 2))
array(1:16, c(2, 2, 4))
array(1:16, c(2, 2, 2, 2))

array(dim=c(2,5,3))	# üres tömb definiálása

a <- array(1:12, c(2,3,2)); a
length(a)
dim(a)
str(a)
class(a)

a[1,3,2]	#  tömb 1. sorának, 3. oszlopának és 2. szintjének eleme

  #########################
  ### KERET (DATAFRAME) ###
  #########################
  # kétdimenziós (r×c) objektum; minden oszlopa ugyanolyan típusú kell legyen, 
  #                              de oszloponként változhat a típus

m1 <- matrix(c(1, 2, 3, 4), nrow = 2, ncol = 2); m1
m2 <- matrix(c("én", "te", "5", "6"), nrow = 2, ncol = 2); m2
as.data.frame(m1)
as.data.frame(m2)

df1 <- rbind(as.data.frame(m1),as.data.frame(m2)); df1
df2 <- cbind(as.data.frame(m1),as.data.frame(m2)); df2

length(df1)
dim(df1)
str(df1)
class(df1)

length(df2)
dim(df2)
str(df2)
class(df2)

df2[1:2, 1]		# dataframe 1-2. sorának 1. eleme
df2[1:2, 4]		# dataframe 1-2. sorának 4. eleme

  ############
  ### LIST ###
  ############
  # objektumok halmaza

v <- c(1, 2, 3)
m <- matrix(1:4, 2)
list(v,m)

l <- list(v,m)
l[[2]]		# list 2. eleme (most mátrix)
l[[2]][1,2]		# list 2. eleme 1. sorának 2. eleme

#####################
## összefésülés:   ##
#####################

# vektorok összefésülése
v1 <- c(1,2,3)
v2 <- 5:6
c(v1,v2)

# matrixok és vektorok összefésülése
m1 <- matrix(1:6, 2)
v1 <- c(1,2,3); rbind(m1, v1)
v2 <- c(5,6); cbind(m1, v2)

#####################
## elnevezések:    ##
#####################

## names()		vektor, dataframe, list
v <- c(1,2,3);
names(v) <- c("Elso", "Második", "Harmadik"); v

## rownames()és colnames()	mátrix, dataframe
m <- matrix(c(1,2,3,4,5,6), ncol=2, nrow=3)
rownames(m) <- c("Sor1","Sor2","Sor3"); m
colnames(m) <- c("Oszlop1", "Oszlop2"); m

# dimnames()	tömb
m1 <- matrix(c(1, 2, 3, 4), nrow = 2, ncol = 2);
m2 <- matrix(c("én", "te", "5", "6"), nrow = 2, ncol = 2);
df1 <- rbind(as.data.frame(m1),as.data.frame(m2))
dimnames(df1) <- list(c("S1", "S2", "S3", "S4"), c("Mi", "Számok"))
df1

df2 <- data.frame(Betuk = letters[seq( from = 1, to = 10 )], Szamok = 1:100)
head(df2)	# ha túl hosszú egy data.frame, csak első néhány sorát kérdezzük le
names(df2)
df2$Szamok	# lekérdezés data.frame-ben, az objektum és az elem nevét $ jel választja el

#####################
## vegyes:         ##
#####################

for (i in 1:5) print(i^2)		# ciklus

x <- 2;  y <- 4
if ((x-y) > 0) 
	{print("x nagyobb, mint y")
} else {
	print("x nem nagyobb y-nál")}


tavolsag <- function(x, y) {		# függvény definiálása
	u = abs(x-y)
	return(u)
}
tavolsag(3, 8)

# a for ciklus és az sapply (lapply) nagyon hasonló
for (x in 1:5) print(x^2)      
sapply(1:5, function(x) x^2)	# egyetlen vektor output
lapply(1:5, function(x) {	# egyetlen lista output
  x^2					# többsoros fv esetén kapcsos zárójel 
})


v <- c(12,14,6,1,8,3)
sort(v)					# sorbarendezés
order(v)
rank(v)

?sort


###################################
## objektumok listázása/törlése: ##
###################################

ls()
rm(list = ls())


#####################
## csomagok:       ##
#####################

# library: R csomag vagy könyvtár. Ezek a programok kiterjesztik az alap R 
# funkcionalitását új függvényekkel, eljárásokkal és statisztikai módszerekkel.

# telepítés (egyszer kell telepíteni):
# 1. install.packages("csomagnév")
# 2. ki kell választani egy CRAN-t (lehet elobb is: chooseCRANmirror())

# csomagnév nevu csomag behívása (minden alkalommal be kell hívni):
# library(csomagnév)

install.packages("lattice")
library(lattice)


#####################
## grafika:        ##
#####################

# alapgrafika, pl. plot, hist, boxplot
# - külön parancsokkal kell a ábra paramétereit beállítani; ábra képernyon, képként menthető

# lattice, pl. xyplot, bwplot, levelplot
# - beállítások függvény segítségével; eredmény: R objektum

# ggplot2, pl. qplot, ggplot
# - az alap grafika és lattice elonyeit tartalmazza


xy <- matrix(c(1,2,3,4,5,7,10,11), nrow = 4, ncol = 2); 
by <- c("A", "A", "B", "B"); 
df <- cbind(as.data.frame(xy),as.data.frame(by)); 
colnames(df) <- c("xval", "yval", "by"); df

x <- seq(0,4,by=0.05)
f <- function(x) {x^3-6*x^2+11*x-6}

ls()

# alapgrafika

  plot(df$xval, df$yval)

  plot(x,f(x))
  plot(x,f(x),type="l",col="blue", lwd=2, main="plot")

# a lattice csomag már volt telepítve
library(lattice)

  abra <- xyplot(yval ~ xval, data=df, groups=by, cex = 1.5, pch=c(18, 19), 
		    main="xyplot (lattice csomag)", ylab="y változó")
  abra

# install.packages("ggplot2")
library(ggplot2)

  qplot(xval, yval, data=df)
  qplot(xval, yval, data=df, color=by, size=I(3), main="qplot (ggplot2)")
  qplot(xval, yval, data=df, fill=by, geom=c("boxplot"), main="qplot (ggplot2)")

  ggplot(df, aes(xval, yval)) + 
    geom_point(aes(col=by), size = 3) +
    labs(y="y változó", 
         x="x változó", 
         title="ggplot (ggplot2 csomag)") 


#####################
## RMarkdown:      ##
#####################
  
# install.packages("rmarkdown")