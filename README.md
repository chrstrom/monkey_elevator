# TTK4235
Søppelkasselignende kode for tilpdat-arbeider utført av inkompetente karer


VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!
Under arbeid, SKAL STAGING ALDRI LIGGE BAK MASTER!!!!!

Gjør dette

1) Gjør lokale endringer i myBranch
2) Så har vi lyst til å merge til staging

// Update lokale endringer
git push origin myBranch

// Merge til staging
git checkout stagig
git merge myBranch
git push origin staging





push til egen branch VANSKELIG MÅTE:

git add <fil>
git commit -m "melding"
git push origin HEAD:<branchName>
  
push til egen branch ENKEL MÅTE:

git checkout <branchName>
add, commit, push, VANLIG MÅTE
  
  
  
Legge til nye filer eller redigere eksisterende filer direkte:
add, commit, push

Legge filer til fra en branch til en annen:
merge
 
