#pragma once

#include <vector>
#include "pch.h"
#include <chrono>

class FramesIterator {
	enum MODES {
		MODE_FORWRARDS = 0,
		MODE_BACKWARDS = 1
	};
	int mode;
	size_t index;
	std::vector<CImage*> frames;
	double frameDelay;
	double frameSwitchTreshold;
	BOOL reversoContext;

	void nextFrame() {
		index++;
		if (index == this->frames.size()) {
			index = 0;
		}
		else if (this->reversoContext && index == this->frames.size() - 1) {
			this->mode = MODE_BACKWARDS;
		}
	}

	void previousFrame() {
		index--;
		if (index == 0) {
			this->mode = MODE_FORWRARDS;
		}
	}

	void clear() {
		this->frameSwitchTreshold = 0;
		size_t size = this->frames.size();
		for (size_t i = 0; i < size; i++) {
			delete this->frames[i];
		}
		this->frameDelay = 0;
	}

	void switchFrame() {
		this->frameSwitchTreshold = 0;
		switch (this->mode) {
		case MODE_FORWRARDS: {
			this->nextFrame();
			break;
		}
		case MODE_BACKWARDS: {
			this->previousFrame();
			break;
		};
		}
	}

public:
	FramesIterator(BOOL reversoContext = TRUE) {
		this->frameDelay = 0;
		this->index = 0;
		this->mode = MODE_FORWRARDS;
		this->frameSwitchTreshold = 0;
		this->reversoContext = reversoContext;
	}

	~FramesIterator() {
		this->clear();
	}

	void Load(const CString paths[], const size_t& size, const double& frameDelay) {
		// must: size >= 2
		this->clear();
		for (size_t i = 0; i < size; i++) {
			CImage* frame = new CImage();
			frame->Load(paths[i]);
			this->frames.push_back(frame);
		}
		this->frameDelay = frameDelay;
		this->mode = MODE_FORWRARDS;
	}

	void Update(const double& timeElapsed) {
		this->frameSwitchTreshold += timeElapsed;

		if (this->frameDelay <= this->frameSwitchTreshold) {
			this->switchFrame();
		}
	}

	CImage* GetFrame() const {
		return this->frames[index];
	}
};
