# TTK4235
Søppelkasselignende kode for tilpdat-arbeider utført av inkompetente karer


VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!VIKTIG!!!
For vår arbeidsflyt, skal staging ALDRI ligge bak master. Dette skjer når man pusher til master direkte.

Push først til egen branch:
git push remote <yourbranch>

Så, når man skal merge:
git checkout staging
git merge <yourbranch>







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
 
