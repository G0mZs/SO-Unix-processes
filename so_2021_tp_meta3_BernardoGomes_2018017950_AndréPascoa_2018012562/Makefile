SUBDIRS = arbitro cliente G_001 G_002 G_003 G_004

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean: 
	rm out/arbitro out/cliente out/G_001 out/G_002 out/G_003 out/G_004
