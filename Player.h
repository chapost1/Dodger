#pragma once

#include "pch.h"

class Player: public CObject {

	DECLARE_SERIAL(Player)

	CString name;
	BOOL isCustomIcon;
	CString iconFileName;
	unsigned long long highestScore;

public:
	Player(): highestScore(0), isCustomIcon(FALSE) {};
	Player(const CString& name);
	Player(const CString& name, const CString& iconFileName);

	void Serialize(CArchive& ar);

	CString GetName() const;
	void SetName(const CString& name);// validate here?

	BOOL IsCustomIcon() const;
	void SetIsCustomIcon(const BOOL& isCustom);

	CString GetIconFileName() const;
	void SetIconFileName(const CString& iconFileName);

	unsigned long long GetHighestScore() const;
	void SetHighestScore(const unsigned long long& score);// set max between current and new score
};

