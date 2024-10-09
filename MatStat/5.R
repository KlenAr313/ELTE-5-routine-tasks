p<-0.01
n<-200
n_sim<-1000

becslesek<-c()

for (i in 1:n_sim) {
  minta<- rgeom(n,p)
  becsles<-n/sum(minta)
  becslesek<-c(becslesek, becsles)
}

mean(becslesek) #torzított mert nem egyenlo 0.01

library(titanic)
data<-titanic_train

eletkorok<-data$Age
eltekorok<-eletkorok[!is.na(eletkorok)]
eletkorok<-eletkorok[eletkorok>1]
