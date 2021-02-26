#include <stdio.h>

#include <proto/exec.h>

#include "Strukturen.h"
#include "Undo.h"
#include "MidiEdit.h"
#include "Sequenzen.h"
#include "EditorNotenGui.h"

struct EDUNDO {
	STRPTR aktion;
	struct EVENTBLOCK *evbl;
	struct EDUNDO *prev;
	struct EDUNDO *next;
};

struct EDUNDO *rootedundo = NULL;
struct EDUNDO *aktedundo = NULL;

struct EVENTBLOCK *KopiereEvbls(struct EVENTBLOCK *evblorig) {
	struct EVENTBLOCK *evblkopie;
	struct EVENTBLOCK *altevbl = NULL;
	struct EVENTBLOCK *evbl = NULL;

	while (evblorig) {
		evblkopie = IExec->AllocVecTags(sizeof(struct EVENTBLOCK), TAG_END);
		if (evblkopie) {
			if (!evbl) evbl = evblkopie;
			IExec->CopyMem(evblorig, evblkopie, sizeof(struct EVENTBLOCK));
			if (altevbl) altevbl->next = evblkopie;
			evblkopie->prev = altevbl;
			evblkopie->next = NULL;
			altevbl = evblkopie;
		}
		evblorig = evblorig->next;
	}
	return(evbl);
}

void AddEdUndo(struct SEQUENZ *seq, STRPTR aktion) {
	struct EDUNDO *neu;
	struct EDUNDO *next;
	
	if (rootedundo) {
		while (rootedundo != aktedundo) {
			EvblsAbschneiden(rootedundo->evbl);
			IExec->FreeVec(rootedundo->evbl);
			
			next = rootedundo->next;
			IExec->FreeVec(rootedundo);
			rootedundo = next;
		}
	}
	
	neu = IExec->AllocVecTags(sizeof(struct EDUNDO), TAG_END);
	if (neu) {
		neu->aktion = aktion;
		neu->evbl = KopiereEvbls(seq->eventblock);
		neu->prev = NULL;
		neu->next = rootedundo;
		if (rootedundo) rootedundo->prev = neu;
		rootedundo = neu;
		aktedundo = neu;
	}
	EdFensterTitel();
}

BOOL EdUndo(struct SEQUENZ *seq) {
	if (aktedundo) {
		if (aktedundo->next) {
			EvblsAbschneiden(seq->eventblock);
			IExec->FreeVec(seq->eventblock);

			aktedundo = aktedundo->next;
			seq->eventblock = KopiereEvbls(aktedundo->evbl);
		
			EdFensterTitel();
			return(TRUE);
		}
	}
	return(FALSE);
}

BOOL EdRedo(struct SEQUENZ *seq) {
	if (aktedundo) {
		if (aktedundo->prev) {
			EvblsAbschneiden(seq->eventblock);
			IExec->FreeVec(seq->eventblock);

			aktedundo = aktedundo->prev;
			seq->eventblock = KopiereEvbls(aktedundo->evbl);
		
			EdFensterTitel();
			return(TRUE);
		}
	}
	return(FALSE);
}

void EntferneAlleEdUndo() {
	struct EDUNDO *akt;
	struct EDUNDO *next;
	
	akt = rootedundo;
	while (akt) {
		EvblsAbschneiden(akt->evbl);
		IExec->FreeVec(akt->evbl);
		
		next = akt->next;
		IExec->FreeVec(akt);
		akt = next;
	}
	rootedundo = NULL;
	aktedundo = NULL;
}

STRPTR EdUndoAktion() {
	if (aktedundo)
		if (aktedundo->next)
			return(aktedundo->aktion);
	return((STRPTR)"--");
}

STRPTR EdRedoAktion() {
	if (aktedundo)
		if (aktedundo->prev)
			return(aktedundo->prev->aktion);
	return((STRPTR)"--");
}
