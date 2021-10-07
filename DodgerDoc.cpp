
// DodgerDoc.cpp : implementation of the CDodgerDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Dodger.h"
#endif

#include "DodgerDoc.h"
#include "DodgerView.h"

#include <propkey.h>

#include "CPlayersSettingsDialog.h"
#include "CGameResetConfirmationDialog.h"

#include <winbase.h>

#include "GameConfig.h"

#include <MMSystem.h>

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDodgerDoc

IMPLEMENT_DYNCREATE(CDodgerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDodgerDoc, CDocument)
	ON_COMMAND(ID_PLAYERS_SETTINGS, &CDodgerDoc::OnMenuPlayersSettingsBtnClick)
	ON_UPDATE_COMMAND_UI(ID_PLAYERS_SETTINGS, &CDodgerDoc::OnVisiblePlayersSettings)
	ON_COMMAND(ID_SETTINGS_NEWGAME, &CDodgerDoc::OnSettingsNewGameClicked)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_NEWGAME, &CDodgerDoc::OnVisibleSettingsNewGameBtn)
END_MESSAGE_MAP()


// CDodgerDoc construction/destruction

CDodgerDoc::CDodgerDoc() noexcept
{
	// TODO: add one-time construction code here
	this->CURR_GAME_STATE = GAME_STATES{ NONE };
	this->SelectedPlayer = nullptr;
	this->selectedPlayerIndex = -1;
	this->loadPlayers();
}

CDodgerDoc::~CDodgerDoc()
{
	this->clearPlayers();
}

BOOL CDodgerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CDodgerDoc serialization

void CDodgerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDodgerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDodgerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDodgerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDodgerDoc diagnostics

#ifdef _DEBUG
void CDodgerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDodgerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDodgerDoc commands

void CDodgerDoc::clearPlayers() {
	int size = this->Players.GetSize();

	for (int i = 0; i < size; i++) {
		Player* player = this->Players.ElementAt(i);
		delete player;
	}

	this->Players.RemoveAll();
}

void CDodgerDoc::setSelectedPlayer(const int& selectedPlayerIndex) {
	this->selectedPlayerIndex = selectedPlayerIndex;

	if (-1 < this->selectedPlayerIndex && this->selectedPlayerIndex < this->Players.GetSize()) {
		this->SelectedPlayer = this->Players.ElementAt(this->selectedPlayerIndex);
	}
	else {
		this->SelectedPlayer = nullptr;
	}
}

void CDodgerDoc::openPlayersSettingsDialog() {
	CPlayersSettingsDialog playersDialog;

	playersDialog.PlayersPtr = &this->Players;

	playersDialog.InitialSelectedPlayerIndex = this->selectedPlayerIndex;

	if (playersDialog.DoModal() == IDOK) {
		this->savePlayers();
		this->setSelectedPlayer(playersDialog.SelectedPlayerIndex);
	}
	else if (
		this->Players.GetSize() < 1 ||
		this->SelectedPlayer == nullptr
	) {
		AfxMessageBox(_T("Notice: To Play, Player Must Be Selected"));
	}
}

void CDodgerDoc::OnMenuPlayersSettingsBtnClick() {
	if (this->CURR_GAME_STATE == GAME_STATES{ RUNNING }) {
		return;
	}

	this->openPlayersSettingsDialog();

	this->loadPlayers();
	this->setSelectedPlayer(this->selectedPlayerIndex);
}

unsigned long long CDodgerDoc::GetHighestPlayersScore() {
	int size = this->Players.GetSize();

	unsigned long long max = 0;

	if (size < 1) return max;

	max = this->Players.ElementAt(0)->GetHighestScore();

	for (int i = 1; i < size; i++) {
		Player* player = this->Players.ElementAt(i);
		if (max < player->GetHighestScore()) {
			max = player->GetHighestScore();
		}
	}

	return max;
}

void CDodgerDoc::savePlayers() {
	CString fileName = _T("./assets/store/players.pl");
	CFile file(fileName, CFile::modeCreate | CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	this->Players.Serialize(ar);
	ar.Close();
	file.Close();

	this->deleteUnusedIcons();
}

void CDodgerDoc::loadPlayers() {
	this->clearPlayers();

	CString fileName = _T("./assets/store/players.pl");
	GetFileAttributes(fileName);
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(fileName) && GetLastError() == ERROR_FILE_NOT_FOUND) {
		return;
	}

	CFile file(fileName, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	this->Players.Serialize(ar);
	ar.Close();
	file.Close();

	this->deleteUnusedIcons();
}

std::vector<CString> getAllFilesInDir(CString dir) {
	std::vector<CString> fileNames;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(dir + _T("/*"), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			fileNames.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	return fileNames;
}

void CDodgerDoc::deleteUnusedIcons() {
	std::vector<CString>::iterator fileNamesIterator, playersUsedIconsIterator;
	std::vector<CString> playersIconsFileNames;
	int size = this->Players.GetSize();

	for (int i = 0; i < size; i++) {
		Player* player = this->Players.ElementAt(i);
		if (player->IsCustomIcon()) {
			playersIconsFileNames.push_back(player->GetIconFileName());
		}
	}

	std::vector<CString> allDirFileNames = getAllFilesInDir(_T("./assets/icons"));

	fileNamesIterator = allDirFileNames.begin();
	while (fileNamesIterator != allDirFileNames.end()) {
		CString filePath = _T("./assets/icons/") + *fileNamesIterator;

		CString fileExtension = PathFindExtension(filePath);
		if (fileExtension == _T(".png")) {
			if (*fileNamesIterator == _T("default.png")) {
				fileNamesIterator++;
				continue;
			}
			BOOL beingUsed = FALSE;
			playersUsedIconsIterator = playersIconsFileNames.begin();
			while (playersUsedIconsIterator != playersIconsFileNames.end()) {
				if (*playersUsedIconsIterator == *fileNamesIterator) {
					beingUsed = TRUE;
					break;
				}
				playersUsedIconsIterator++;
			}

			if (!beingUsed) {
				DeleteFile(filePath);
			}
		}

		fileNamesIterator++;
	}
}

void CDodgerDoc::stopBackgroundMusic() {
	if (lastBackGroundMusic != "") {
		CString b = _T("stop ") + lastBackGroundMusic;
		mciSendString(b, NULL, 0, 0);
		lastBackGroundMusic = "";
	}
}

void CDodgerDoc::playMusic(CString FilePath, BOOL repeat) {
	CString a = _T("open ") + FilePath + _T(" type mpegvideo");
	mciSendString(a, NULL, 0, 0);
	CString b = _T("play ") + FilePath;
	if (repeat) {
		b += _T(" repeat");
	}
	b += _T(" from 0");
	int error = mciSendString(b, NULL, 0, 0);
	if (error) {
		AfxMessageBox(_T("Sad music! exiting.."));
		exit(1);
	}
}

void CDodgerDoc::playBackgroundMusic(CString FilePath, BOOL repeat) {
	playMusic(FilePath, repeat);
	this->lastBackGroundMusic = FilePath;
}

void CDodgerDoc::Renewal(const int& newGameState) {
	const int prevGameState = this->CURR_GAME_STATE;
	if (prevGameState == newGameState) return;

	if (newGameState == GAME_STATES{ GAME_OVER }) {
		this->savePlayers();
	}

	this->updateMusic(newGameState);

	this->CURR_GAME_STATE = newGameState;
}

void CDodgerDoc::updateMusic(const int& newGameState) {
	const int prevGameState = this->CURR_GAME_STATE;
	if (prevGameState == newGameState) return;

	// one time music on specific scenarios:
	if (prevGameState == GAME_STATES{ GAME_READY } && newGameState == GAME_STATES{ RUNNING }) {
		CString a = _T("./assets/music/enter.mp3");
		playMusic(a, FALSE);
	}
	else if (prevGameState == GAME_STATES{ RUNNING } && newGameState == GAME_STATES{ GAME_OVER }) {
		CString a = _T("./assets/music/crash.mp3");
		playMusic(a, FALSE);
	}
	else if (prevGameState == GAME_STATES{ GAME_OVER } && newGameState == GAME_STATES{ RUNNING }) {
		CString a = _T("./assets/music/enter.mp3");
		playMusic(a, FALSE);
	}

	// background musics:
	stopBackgroundMusic();
	if (newGameState == GAME_STATES{ INSTRUCTIONS }) {
		CString a = _T("./assets/music/instructions.mp3");
		playBackgroundMusic(a, FALSE);
	}
	else if (newGameState == GAME_STATES{ RUNNING }) {
		CString a = _T("./assets/music/running.mp3");
		playBackgroundMusic(a, TRUE);
	}
	else if (newGameState == GAME_STATES{ GAME_READY }) {
		CString a = _T("./assets/music/ready.mp3");
		playBackgroundMusic(a, TRUE);
	}
	else if (newGameState == GAME_STATES{ GAME_OVER }) {
		CString a = _T("./assets/music/game_over.mp3");
		playBackgroundMusic(a, TRUE);
	}
}

void CDodgerDoc::OnVisiblePlayersSettings(CCmdUI* pCmdUI) {
	const BOOL enable = this->CURR_GAME_STATE != GAME_STATES{ RUNNING };
	pCmdUI->Enable(enable);
}

void CDodgerDoc::OnSettingsNewGameClicked() {
	CGameResetConfirmationDialog confirmationDialog;

	if (confirmationDialog.DoModal() == IDOK) {
		POSITION pos = GetFirstViewPosition();
		if (pos != NULL) {
			CDodgerView* pView = NULL;
			pView = dynamic_cast<CDodgerView*>(GetNextView(pos));
			if (pView) {
				this->SelectedPlayer = nullptr;
				this->selectedPlayerIndex = -1;
				CString fileName = _T("./assets/store/players.pl");
				DeleteFile(fileName);
				this->loadPlayers();
				this->deleteUnusedIcons();
				pView->ResetGame();
			}
		}
	}
}

void CDodgerDoc::OnVisibleSettingsNewGameBtn(CCmdUI* pCmdUI) {
	const BOOL enable = this->CURR_GAME_STATE != GAME_STATES{ RUNNING };
	pCmdUI->Enable(enable);
}
