#8 / 2
A_busz <- c(2.8, 2.85, 2.9, 2.95, 3.00, 3.10, 3.40 );
B_busz <- c(2.75, 2.8, 2.85, 2.85, 2.9, 2.95, 2.95, 3.15);

var.test(A_busz, B_busz)
#p = 0.2161
F_stat <-var(A_busz)/var(B_busz)
(1-pf(F_stat, length(A_busz)-1, length(B_busz)-1))*2

# a p-ertek nagyobb mint .05 szinifikancia szint
# tehát nem tudjuk elvetni a nullhipotézis , a szorások nem különbözőek
# tehát  kétmintás t-próba kell a várható érték

t.test(A_busz, B_busz,
       'greater', paired = FALSE) # mert a h1: MuA > MuB <=> MuA - MuB > 0
#p-value = 0.1411 > .05 szignifiakncia szint, 
# nem tudjuk elvetni a nullhiptézisünket
# A B busz atlagos menetideje nem kisebb, mint az A busz esetén.

# 8/4

reszvenyHozamo <- c(22.89, 29.01, 43.74, 7.79, 22.33, -11.15, -49.01, -16.4, 8.53, -18.27, 69.98, 52.75, 7.87, 25.83, -2.54, -12.11, -39.45, -28.74, -53.33, 49.35)
Mu <- 10
t_probafv <- (mean(reszvenyHozamo)-Mu)/(sd(reszvenyHozamo)/sqrt(length(reszvenyHozamo)))
t_probafv
pt(t_probafv, df=length(reszvenyHozamo)-1)
# peretek=  0.2815159 > .05 igy NEM vethetjuk el a nullhipünket tehat a a menedzser nem teljsítette alul a piaci átlagot

#b
sigma_0 = 30
khiNegyzet_pf <- (length(reszvenyHozamo)-1)*(sd(reszvenyHozamo)^2/(sigma_0^2))
#p ertek:
(1-pchisq(khiNegyzet_pf, length(reszvenyHozamo)-1))*2
#  0.3089194 nem kisebb mint a .05 szignifikancia szint
# nem külünbozik szgnifik a 30%tól

#9/1

UszE = c(22.55, 22.5, 22.38, 22.49, 22.46, 22.51, 22.55, 22.47, 22.38, 22.4, 22.44, 22.36, 22.56, 22.61, 22.59, 22.5, 22.36, 22.41, 22.4, 22.52)
UszU = c(22.27, 22.26, 22.19, 22.34, 22.34, 22.33, 22.12, 22.26, 22.25, 22.32, 22.16, 22.22, 22.46, 22.22, 22.17, 22.21, 22.43, 22.12, 22.43, 22.39)
UszK = UszE - UszU
tProbF = mean(UszK)/sd(UszK)/sqrt(length(UszK))
tProbF
1-pt(tProbF, length(UszK) - 1)
# a p = .37 > .05 szignifikancia szint, tehat nem tudjuk elvetni a nullhip, nem tortents szignifikans javulás az uszas idoben




