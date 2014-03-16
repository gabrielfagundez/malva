default:
	@echo "Make what? (all, libs, skeletons,  clean)"

all:
	@echo "Making only libs since there are not migrated code. Don't worry, everything will work."
	(cd inc ; rm -f Mallba)
	(cd inc ; ./inc.env)
	(cd src ; make all)
	(cd lib ; make all)

libs:
	(cd inc ; rm -f Mallba)
	(cd inc ; ./inc.env)
	(cd src ; make all)
	(cd lib ; make all)

skeletons:
	(cd rep ; make all)

clean:
	(cd inc ; rm -f Mallba)
	(cd src ; make clean)
	(cd lib ; make clean)
	(cd rep ; make clean)
