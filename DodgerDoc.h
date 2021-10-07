
// DodgerDoc.h : interface of the CDodgerDoc class
//

#pragma once

#include "Player.h"
#include "Observer.h"

class CDodgerDoc : public CDocument, public IObserver<int> {
	int CURR_GAME_STATE;
	CString lastBackGroundMusic;

	void openPlayersSettingsDialog();

	void savePlayers();
	void loadPlayers();
	void clearPlayers();
	void setSelectedPlayer(const int& selectedPlayerIndex);

	void deleteUnusedIcons();

	void stopBackgroundMusic();
	void playBackgroundMusic(CString FilePath, BOOL repeat);
	void playMusic(CString FilePath, BOOL repeat);
	void updateMusic(const int& newGameState);
public:
	CTypedPtrArray<CObArray, Player*> Players;

protected: // create from serialization only
	CDodgerDoc() noexcept;
	DECLARE_DYNCREATE(CDodgerDoc)

// Attributes
public:
	Player* SelectedPlayer;
	int selectedPlayerIndex;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	void Renewal(const int& newGameState) override;

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDodgerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnMenuPlayersSettingsBtnClick();

	unsigned long long GetHighestPlayersScore();
	afx_msg void OnVisiblePlayersSettings(CCmdUI* pCmdUI);
	afx_msg void OnSettingsNewGameClicked();
	afx_msg void OnVisibleSettingsNewGameBtn(CCmdUI* pCmdUI);
};
