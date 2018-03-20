CC 		= gcc
IDIR 	= .
CFLAGS 	= -ansi -Wall -g # -I$(IDIR)
OBJDIR 	= obj

#A4
A4_SRCDIR 		= .
A4_IDIR 		= .
A4_TESTDIR 		= .
A4_BINDIR 		= .

A4_AA_OBJ 		= $(OBJDIR)/addAuthor.o
A4_AA_LIBS		= -lstream -L $(A4_SRCDIR)/ -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

A4_POST_OBJ		= $(A4_SRCDIR)/post.o $(OBJDIR)/twoDArray.o
A4_POST_LIBS	= -lstream -L $(A4_SRCDIR)/ -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

A4_DB_OBJS		= $(OBJDIR)/db.o $(OBJDIR)/dbutil.o
A4_DB_LIBS 		= -lmysqlclient -L/usr/lib/x86_64-linux-gnu/
A4_DB_EXE		= $(A4_BINDIR)/db

A4_STREAM_OBJ	= $(A4_SRCDIR)/stream.o $(OBJDIR)/dbutil.o

#A3
IDLOBJ 			= $(OBJDIR)/IDL.o
IDLLIBS 		=
IDLEXE 			= IDL
A3_SRCDIR 		= .
A3_IDIR 		= .
A3_TESTDIR 		= .
A3_BINDIR 		= .
WEBVIEWSCRIPT 	= webview.py
INDEXPHP 		= index.php
INDEXCONFIG		= index.wpml

#A2
A2_SRCDIR 	= a2src
A2_IDIR 	= a2src
A2_TESTDIR 	= a2src
A2_BINDIR 	= a2src

#VIEW
VIEWSCRIPT = view.py

#POST
POSTOBJ 	= $(OBJDIR)/post.o $(OBJDIR)/twoDArray.o
POSTLIBS 	= -lstream -L $(A2_SRCDIR)/
POSTEXE 	= post
POSTTESTOBJ = $(OBJDIR)/testStream.o
POSTTESTEXE = testStream

#AA
AAOBJ 	= $(OBJDIR)/addAuthor.o
AALIBS 	= -lstream -L $(A2_SRCDIR)/
AAEXE 	= addauthor

#STREAM
STREAMLIB 	= libstream.a
MESSAGESDIR = $(A3_SRCDIR)/messages

#A1
A1_SRCDIR 	= a1src
A1_IDIR 	= a1src
A1_BINDIR 	= a1src
A1_TESTDIR 	= a1src
A1OBJ 		= $(OBJDIR)/a1.o $(OBJDIR)/converter.o $(OBJDIR)/twoDArray.o $(OBJDIR)/tokenizer.o $(OBJDIR)/codeToken.o
A1LIBS 		=
A1EXE 		= a1

all: A3 A4

#A4
A4: A4AA A4POST A4DB

#A4AA
A4AA: $(A4_BINDIR)/$(AAEXE)
$(A4_BINDIR)/$(AAEXE): $(OBJDIR) $(A4_AA_OBJ) A4STREAM
	$(CC) $(A4_AA_OBJ) $(A4_AA_LIBS) -o $@

#A4POST
A4POST: $(A4_BINDIR)/$(POSTEXE)
$(A4_BINDIR)/$(POSTEXE): $(OBJDIR) $(A4_POST_OBJ) A4STREAM
	$(CC) $(A4_POST_OBJ) $(A4_POST_LIBS) -o $@

$(A4_SRCDIR)/post.o: $(A4_SRCDIR)/post.c $(A4_IDIR)/post.h $(A1_IDIR)/twoDArray.h $(A4_IDIR)/dbutil.h
	$(CC) -c $< $(CFLAGS) -I$(A4_IDIR) -I$(A1_IDIR) -o $@

$(A4_SRCDIR)/post.c: $(A4_SRCDIR)/post.cc $(A1_BINDIR)/$(A1EXE)
	$(A1_BINDIR)/$(A1EXE) $<

#A4STREAM
A4STREAM: $(A4_SRCDIR)/$(STREAMLIB)
$(A4_SRCDIR)/$(STREAMLIB): $(A4_STREAM_OBJ)
	ar cr $@ $^

$(A4_SRCDIR)/stream.o: $(A4_SRCDIR)/stream.c $(A4_IDIR)/stream.h $(A4_IDIR)/dbutil.h
	$(CC) -c $< $(CFLAGS) -I$(A4_IDIR) -I$(A1_IDIR) -o $@

#A4DB
A4DB: $(A4_BINDIR)/$(A4_DB_EXE)
$(A4_BINDIR)/$(A4_DB_EXE): $(OBJDIR) $(A4_DB_OBJS) 
	$(CC) $(A4_DB_OBJS) $(A4_DB_LIBS) -o $@

$(OBJDIR)/db.o: $(A4_SRCDIR)/db.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR)/dbutil.o: $(A4_SRCDIR)/dbutil.c $(A4_IDIR)/dbutil.h
	$(CC) -c $< $(CFLAGS) -o $@

#A3
A3: WEBVIEW WEBPAGE

#WEBVIEW
WEBVIEW: $(A3_SRCDIR)/$(WEBVIEWSCRIPT)
	chmod +x $(A3_SRCDIR)/$(WEBVIEWSCRIPT)
	
WEBPAGE: $(A3_SRCDIR)/$(INDEXPHP)
$(A3_SRCDIR)/$(INDEXPHP): IDLPROG $(A3_SRCDIR)/$(INDEXCONFIG) $(A3_SRCDIR)/indexphp.php
	$(A3_BINDIR)/$(IDLEXE) $(A3_SRCDIR)/$(INDEXCONFIG) > $(A3_SRCDIR)/$(INDEXPHP)

IDLPROG: $(A3_BINDIR)/$(IDLEXE)
$(A3_BINDIR)/$(IDLEXE): $(OBJDIR) $(IDLOBJ)
	$(CC) $(IDLOBJ) $(IDLLIBS) -o $@

$(OBJDIR)/IDL.o: $(A3_SRCDIR)/IDL.c $(IDIR)/IDL.h
	$(CC) -c $< $(CFLAGS) -o $@

#A2
A2: A2POST A2AA VIEW

#VIEW
VIEW: $(A2_SRCDIR)/$(VIEWSCRIPT)
	chmod +x $(A2_SRCDIR)/$(VIEWSCRIPT)

#A2AA
A2AA: $(A2_BINDIR)/$(AAEXE)
$(A2_BINDIR)/$(AAEXE): $(OBJDIR) $(MESSAGESDIR) $(AAOBJ) $(A2_SRCDIR)/$(STREAMLIB)
	$(CC) $(AAOBJ) $(AALIBS) -o $@

$(OBJDIR)/addAuthor.o: $(A2_SRCDIR)/addAuthor.c $(A2_IDIR)/addAuthor.h
	$(CC) -c $< $(CFLAGS) -o $@

#A2POST
A2POST: $(A2_BINDIR)/$(POSTEXE)
$(A2_BINDIR)/$(POSTEXE): $(OBJDIR) $(MESSAGESDIR) $(POSTOBJ) $(A2_SRCDIR)/$(STREAMLIB)
	$(CC) $(POSTOBJ) $(POSTLIBS) -o $@

$(OBJDIR)/post.o: $(A2_SRCDIR)/post.c $(A2_IDIR)/post.h
	$(CC) -c $< $(CFLAGS) -Ia1src -o $@

$(A2_SRCDIR)/post.c: $(A2_SRCDIR)/post.cc $(A1_BINDIR)/$(A1EXE)
	$(A1_BINDIR)/$(A1EXE) $<
#mv -f post.c $(A2_SRCDIR) || true

#A2POSTTEST
A2POSTTEST: $(A2_BINDIR)/$(POSTTESTEXE)
$(A2_BINDIR)/$(POSTTESTEXE): $(MESSAGESDIR) $(POSTTESTOBJ) A2STREAM
	$(CC) $(POSTTESTOBJ) $(POSTLIBS) -o $@

$(OBJDIR)/testStream.o: $(A2_TESTDIR)/testStream.c
	$(CC) -c $< $(CFLAGS) -o $@

#STREAM
A2STREAM: $(A2_SRCDIR)/$(STREAMLIB)
$(A2_SRCDIR)/$(STREAMLIB): $(OBJDIR)/stream.o $(OBJDIR)/twoDArray.o
	ar cr $@ $(OBJDIR)/stream.o $(OBJDIR)/twoDArray.o

$(OBJDIR)/stream.o: $(A2_SRCDIR)/stream.c $(A2_IDIR)/stream.h
	$(CC) -c $< $(CFLAGS) -Ia1src -o $@

#A1
A1: $(A1_BINDIR)/$(A1EXE)

$(A1_BINDIR)/$(A1EXE): $(OBJDIR) $(A1OBJ)
	$(CC) $(A1OBJ) $(A1LIBS) -o $@

$(OBJDIR)/a1.o: $(A1_SRCDIR)/a1.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR)/converter.o: $(A1_SRCDIR)/converter.c $(A1_IDIR)/converter.h
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR)/tokenizer.o: $(A1_SRCDIR)/tokenizer.c $(A1_IDIR)/tokenizer.h
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR)/codeToken.o: $(A1_SRCDIR)/codeToken.c $(A1_IDIR)/codeToken.h
	$(CC) -c $< $(CFLAGS) -o $@

#TWODARRAY
$(OBJDIR)/twoDArray.o: $(A1_SRCDIR)/twoDArray.c $(A1_IDIR)/twoDArray.h
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(MESSAGESDIR):
	mkdir $(MESSAGESDIR)

#PHONY
.PHONY: clobber clean clearMessages

clobber: clean
	rm -f $(A4_BINDIR)/$(A4_DB_EXE)
	rm -f $(A4_BINDIR)/$(POSTEXE)
	rm -f $(A4_BINDIR)/$(AAEXE)
	rm -f $(A3_BINDIR)/$(IDLEXE)
	rm -f $(A2_BINDIR)/$(AAEXE)
	rm -f $(A2_BINDIR)/$(POSTEXE)
	rm -f $(A2_BINDIR)/$(POSTTESTEXE)
	rm -f $(A1_BINDIR)/$(A1EXE)

clean:
	rm -f $(A4_SRCDIR)/*.o
	rm -f $(A4_SRCDIR)/$(STREAMLIB)
	rm -f $(A2_SRCDIR)/$(STREAMLIB)
	rm -f $(A2_SRCDIR)/post.c
	rm -f $(OBJDIR)/*.o
	rm -f *.stackdump

clearMessages:
	rm -f $(MESSAGESDIR)/*

