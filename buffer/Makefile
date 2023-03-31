# CS 240 Buffer


BIN = laptop.bin
EXPLOITS = exploit1.hex exploit2.hex exploit3.hex exploit4.hex
BYTES = .test.bytes

test: id.txt
	@for e in $(EXPLOITS); do if [ -e $$e ]; then echo '##' $$e '################################################################'; ./hex2raw.bin < $$e > $(BYTES) && gdb -q -x test.gdb $(BIN); echo; rm -f $(BYTES); fi; done

id.txt:
	@cs240 id > $@
cookie.txt: id.txt
	@./id2cookie.bin $$(cat $<) > $@
	@git add $@ $<
	@git commit -m "[make cookie]" $@ $<
cookie: id.txt cookie.txt
	@echo "The cookie for $$(cat id.txt) is:"
	@cat cookie.txt
	@echo

.PHONY: cookie
