#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

int procB(void) {
    // Process B writing to C
    while (!std::cin.eof()) {
		// read a line of input until EOL and store in a string
		std::string line;
		std::getline(std::cin, line);
		if (line.size () > 0)
			std::cout << line << std::endl;
    }
    std::cout << "[B] saw EOF" << std::endl;
    return 0;
}



int main (int argc, char* argv[]) {
//int main(void){
    std::vector<pid_t> kids;
	int status;
	//int i;
	/*const char* arg_gen[argc+1]; 
	arg_gen[0] = (char*)"./rgen";
	arg_gen[argc] = nullptr;
	for(int i=1; i<argc; i++){
	  arg_gen[i] = argv[i];
	}*/
    // create two pipes
    int GentoPy[2];
	int PytoC[2];
    pipe(GentoPy);
	pipe(PytoC);


    pid_t child_pid;
	
		
	child_pid = fork ();
	if (child_pid == (pid_t) 0)
	{
		// redirect stdout to the pipe
		dup2(GentoPy[1], STDOUT_FILENO);
		close(GentoPy[0]);
		close(GentoPy[1]);     // Close this too!
		close(PytoC[0]);
		close(PytoC[1]); 
		//execvp ("./rgen", "./rgen", argv);
		execv ("./rgen", argv);
		std::cerr << "Error: Could not run rgen\n";
		return 1;
	}
	else {
		kids.push_back(child_pid);
		child_pid = fork();
		if (child_pid == 0)
		{
			// redirect stdin from the pipe
			dup2(GentoPy[0], STDIN_FILENO);
			close(GentoPy[0]);
			close(GentoPy[1]);
			dup2(PytoC[1], STDOUT_FILENO);
			close(PytoC[0]);
			close(PytoC[1]);  
			execlp ("python", "python", "a1-ece650.py", nullptr);
			std::cerr << "Error: Could not run python file\n";
			return 1;
		}
		else{
			kids.push_back(child_pid);
			child_pid = fork();
			if (child_pid == 0)
			{
				// redirect stdin from the pipe
				dup2(PytoC[0], STDIN_FILENO);
				close(PytoC[0]);
				close(PytoC[1]);
				close(GentoPy[0]);
				close(GentoPy[1]);
				execlp ("./a2-ece650", "./a2-ece650", nullptr);
				std::cerr << "Error: Could not run a2-ece650\n";
				return 1;	
			}
			else{
				kids.push_back(child_pid);
				child_pid = fork();
				if (child_pid == 0)
				{
					// redirect stdin from the pipe
					dup2(PytoC[1], STDOUT_FILENO);
					close(PytoC[0]);
					close(PytoC[1]);
					close(GentoPy[0]);
					close(GentoPy[1]);
					return procB();	
				}
				
			}
			
		}
    }
	kids.push_back(child_pid);
	close(PytoC[0]);
	close(PytoC[1]);
	close(GentoPy[0]);
	close(GentoPy[1]);
	//for (i = 0; i < 4; i++)
	//	std::cout<<"Waiting for process to end:" <<i;
	wait(&status);	
	for (pid_t k : kids) {
        //int status;
        kill (k, SIGTERM);
        //waitpid(k, &status, 0);
    }
		
    return 0;

}
