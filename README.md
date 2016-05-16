# League of Legends Championship Management System - Version 1
This is an implementation of a program that maintains a database for a League of Legends Championship using just a data file and indexes in memory. It was implemented as part of an exercise for a Data Structure class. 

The database contains the following fields:

- Primary Key: Composed by the first letter of Blue Team, first letter of Red Team, first two letters of the MVP (Most Valuable Player), and the date of the match composed by day and month. Ex: FTFE0205 (8 bytes)
- Blue Team: Name of the blue team, ex: Fnatic (Variable Size)
- Red Team: Name of the red team, ex: Team Solo Mid (Variable Size)
- Match Date: Date of the match, ex: DD/MM/AAAA (10 bytes)
- Match Duration: Duration of the match in minutes and seconds, ex MM:SS (5 bytes)
- Winner Team: Name of the winner team. (Variable Size)
- Match Score: Score of the match, ex: 15, 06 (4 bytes) 
- MVP Nickname: Nickname of the Most Valuable Player, ex: Febiven (Variable Size)

Each entry in the database will have a maximum storage of 192 bytes per entry. These fields are stored in a data file as shown below:

```
FTFE0705@Fnatic@Team SoloMid@07/05/2015@31:47@Fnatic@15@06@Febiv
en@#############################################################
################################################################
SEDE1005@SKTelecom T1@Edward Gaming@10/05/2015@37:38@Edward Gami
ng@09@25@Deft@##################################################
################################################################
AHWE2607@ahq e-Sports Club@HongKong Esports@26/07/2015@39:09@ahq
e-Sports Club@20@14@westdoor@##################################
################################################################
PIMY0808@PaiN Gaming@INTZ@08/08/2015@42:55@PaiN Gaming@10@07@Myl
on@#############################################################
################################################################
JNCH0209@Jin Air Green Wings@NaJin e-mFire@02/09/2015@39:51@Jin
Air Green Wings@08@03@Chei@#####################################
################################################################
```

This solution implements three indexes:
- Primary Index containing the Primary Key and offset of the entry in the file
- Secondary Winner Index relating a Winner Team to its Primary Key
- Secondary MVP Index relating a MVP to its Primary Key

Inside the program, the following options are provided to the final user:
- 1. Create new match
- 2. Remove match using Primary Key
- 3. Update Match Duration using Primary Key 
- 4. Search matches:
  - Using Primary Key
  - Using Winner
  - Using MVP
- 5. List all matches:
  - Using Primary Key
  - Using Winner
  - Using MVP
- 6. Free Memory
