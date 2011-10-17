MAKE=make
BINA=ydkrc
Q=@

all: $(BINA)

ydkrc:
	$(Q)cd client/src/; $(MAKE)

#etafeld:
#	$(Q)cd server; $(MAKE)

clean:
	$(Q)rm -f $(BINA) *.bak *~
	$(Q)cd client/src/; $(MAKE) clean
	#$(Q)cd server; $(MAKE) clean
	$(Q)cd common/src/; $(MAKE) clean
