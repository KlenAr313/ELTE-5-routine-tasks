#install.packages("readxl")
library("readxl")
incU1M22 <- read_excel("./MatStat/Bead/TEIR_inc_u_1m_22.xlsx")
egyeni_full = read_excel("./MatStat/Bead/Egyéni_szavazás_szkjkv_22.xlsx")
egyeni_full
egyeni = egyeni_full[,c('TELEPÜLÉS', 'VÁLASZTÓPOLGÁR', 'ÉRVÉNYES', 'SZERVEZET', 'SZAVAZAT')]
egyeni

telepules = egyeni[(is.na(egyeni[, c('TELEPÜLÉS')]) == 0),c('TELEPÜLÉS', 'VÁLASZTÓPOLGÁR', 'ÉRVÉNYES')]
telepules
fidesz = egyeni[(is.na(egyeni[, c('SZERVEZET')]) == 0 & egyeni[, c('SZERVEZET')] == 'FIDESZ - MAGYAR POLGÁRI SZÖVETSÉG-KERESZTÉNYDEMOKRATA NÉPPÁRT'), 'SZAVAZAT']
fidesz

cleaned = cbind(telepules, fidesz)
cleaned
