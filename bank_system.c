#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <time.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <sys/ipc.h>
 

void deposit(int* BankAccount) {
	int random_Money = (rand() % 100);
  
	if ((random_Money % 2) == 0) {
    
		*BankAccount += random_Money;
    
		printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", random_Money, *BankAccount);
	
  } else {
		printf("Dear old Dad: Doesn't have any money to give\n");
	}
}

void withdrawal(int* BankAccount) {
	int withdrawn = (rand() % 50);
  
	printf("Poor Student needs $%d\n", withdrawn);
	
  if (*BankAccount >= withdrawn) {
		  
    *BankAccount -= withdrawn;
    
		printf("Poor Student: Withdraws $%d / Balance = $%d\n", withdrawn, *BankAccount);
	
  } else {
		printf("Poor Student: Not Enough Cash ($%d)\n", *BankAccount);
	}
}

int  main(int  argc, char *argv[]) {
	pid_t  pid;	
  int Shm_ID;
	int *Shm_PTR;
		
		int* BankAccount;
		int* Turn;

		Shm_ID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
		if (Shm_ID < 0) {
			printf("///////shmget error///////\n");
			exit(1);
		}
  
		printf("Process has recieved Shared Memory of 2 integers\n");

		Shm_PTR = (int *) shmat(Shm_ID, NULL, 0);

		if (*Shm_PTR < 0) {
			printf("///////shmat error (server)///////\n");
			exit(1);
		}
  
		printf("Process has attached the Shared Memory\n");

    Shm_PTR[1] = 0;
		Shm_PTR[0] = 0;
		
		
		BankAccount = &Shm_PTR[0];
		Turn = &Shm_PTR[1];

		srand(time(0));

		pid = fork();
		
		if (pid > 0) { 
			int count = 0;

			while (count < 25) {
        
				int random_Sleep = (rand() % 1);
				sleep(random_Sleep);

				while (*Turn == 0); 
        
				if (*BankAccount <= 100) { 
					deposit(BankAccount);
				} else {
					printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", *BankAccount);
				}*Turn = 0; 
				count++; 
			}
				wait(&pid);
      
				printf("Process has detected the completion of its child\n");
				shmdt((void *) Shm_PTR);
      
				printf("Process has detached its Shared Memory\n");
				shmctl(Shm_ID, IPC_RMID, NULL);
      
				printf("Process has removed its Shared Memory\n");
				printf("Exits Process\n");
				exit(0);
      
		} else {

      
      for(int count = 0;count < 25;count++){
        
				int random_Sleep = (rand() % 1);
        
				sleep(random_Sleep);
        
				while (*Turn == 1);
				withdrawal(BankAccount); 
				*Turn = 1; 
				
			}
			exit(0);
		}
}

