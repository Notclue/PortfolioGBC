#pragma once
enum HardwareRevision {
	//TODO: fill out more revisions
	DMG,
	CGB
};

static constexpr __forceinline bool RevisionIsColor( HardwareRevision rev ) {
	return rev == CGB;
}