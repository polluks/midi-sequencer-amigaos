void InitController(void);
void ZeichneControllerSpalten(void);
void ZeichneContr(int16 n, struct EVENT *lcev, struct EVENT *fcev, BOOL mitte, BOOL del);
void ZeichneContrVorschau(int16 n, struct EVENT *ev, BOOL mitte);
void ZeichneControllerSpur(int16 n);
void ZeichneControllerFeld(BOOL anders);
void ContrSpurenEinpassen(void);
void ZeichneEdContrInfobox(void);
int16 PunktContrSpur(int16 my);
int8 PunktContrWert(int16 my, int16 cs, BOOL onoff);
void ContrSpurAktivieren(int16 n);
