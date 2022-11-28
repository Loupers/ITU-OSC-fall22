# Datalab
In this assignment you will have to edit a file written in the C programming
language to get a better grasp at bitwise operations.

This README details how you should handle this assignment:

1. [Setting up datalab](#setting-up-datalab)
2. [Doing the assignment](#doing-the-assignment)
3. [Submitting your work](#submitting-your-work)


## Setting up datalab
You must fork this repository to your own user account and work on the
assignment from that fork. When you are done, you should create a pull request
against the original repository.


### Fork repository
The following images illustrate how to fork the course repository.

First press the Fork-button on the lab repository:
![Fork](./img/Fork.png)


You may have to select the organisation or owner of the fork.<br>
Select your ITU username:
![ChooseUser](./img/ChooseUser.png)


When forked, we need to get the repository’s URL to clone from.<br>
If you press `Use HTTPS` at the top of the popup box (see image below), you
will authenticate against github with ITU username and password instead of
ssh keys (see [this link](https://devconnected.com/how-to-setup-ssh-keys-on-github/)
for information on how to use SSH keys to interact with GitHub).
![GetCloneUrl](./img/GetCloneUrl.png)


### Setup your working copy of the repo
Switch to a terminal (logged in to the course server, for instance) and do the
following:
```
# Download git repository (replace pawp with your own user name)
git clone git@github.itu.dk:pawp/20-Lab1-datalab.git Lab1-datalab

# Change into cloned directory
cd Lab1-datalab

# Checkout your group’s branch (replace XX with your group number)
git checkout -b groupXX

# Make initial push to remote to set upstream clone URL (replace XX again)
git push -u origin groupXX
```

I have recorded myself doing this on [this screencast](https://asciinema.org/a/YM6ubxi5q7xqoAM89AxK5GuaD),
so you can verify that your process looks similar, if you become unsure.


## Doing the assignment
You should now have access to the course files on your user account and have set
up a tracking branch corresponding to your username on your own fork of the
repository.<br>
You are now ready to edit `bits.c` according to the instructions in the
assignment [pdf file](./datalab.pdf) and the handout’s original
[README](./README).

Example workflow:
```
# Open and edit the assignment file in vim
vim bits.c

# Run the dlc compiler (read README for more information)
# If this step fails your assignment does not compile!
./dlc -e bits.c

# Create a test program when compilation succeeds
make clean; make btest

# Run all provided tests
./btest

# Assuming tests for the edited function passes, commit them.
git add bits.c
git commit -m “Solved exercise foo by doing bar.”
```    

It is good practice to do the last step regularly.<br>
Try making a habit out of committing each new thing that you try out -preferably
when it works- to more effectively use your revision history.


## Submitting your work
When you are ready to submit the assignment, you should commit any outstanding
changes to `bits.c` and push them to your fork. Then, create a pull request to
the “forked-from” repository, as illustrated below.


### Push changes to your branch
Add changed `bits.c`-file to staging area, then commit staged file:
```
git add bits.c
git commit -m “Hand-in from groupXX (pawp). Final version.”:
```

Push changes to your fork:
```
git push
```
   
### Create a pull request
On ITU’s github, on your fork’s repository frontpage
(`https://github.itu.dk/<YOUR_ITU_USERNAME>/21-Lab1-datalab`) you can see how
many commits you have made since forking and there is a little button in the
same line that allows you to make pull requests against the repository and
branch that was forked from:

![New PR](./img/MakePullRequest.png)


This will take you to the following page, where you should:

1. Enter your full group name (`groupXX`, where `XX` is your group number) in the title.
2. Type in comments to your handins (sorta like assignment comments on LearnIT)
3. Press the green `Create pull request`-button<br>
   (alternatively, you can press the downward arrow right next to it and only
   open the PR in `DRAFT` mode indicating that it is not yet ready for TA
   review).


![Open PR](./img/OpenPullRequest.png)
