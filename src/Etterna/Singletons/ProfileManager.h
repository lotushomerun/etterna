#ifndef ProfileManager_H
#define ProfileManager_H

#include "Etterna/Models/Misc/Difficulty.h"
#include "Etterna/Models/Misc/GameConstantsAndTypes.h"
#include "Etterna/Models/Misc/PlayerNumber.h"
#include "Etterna/Models/Misc/Preference.h"
#include "Etterna/Models/Misc/Profile.h"
#include "arch/LoadingWindow/LoadingWindow.h"

class Song;
class Steps;
class Style;
struct HighScore;
struct lua_State;
class ProfileManager
{
  public:
	ProfileManager();
	~ProfileManager();

	void Init(LoadingWindow* ld);

	bool FixedProfiles() const; // If true, profiles shouldn't be added/deleted

	// local profiles
	void UnloadAllLocalProfiles();
	void RefreshLocalProfilesFromDisk();
	void RefreshLocalProfilesFromDisk(LoadingWindow* ld);
	const Profile* GetLocalProfile(const RString& sProfileID) const;
	Profile* GetLocalProfile(const RString& sProfileID)
	{
		return (Profile*)((const ProfileManager*)this)
		  ->GetLocalProfile(sProfileID);
	}
	Profile* GetLocalProfileFromIndex(int iIndex);
	RString GetLocalProfileIDFromIndex(int iIndex);

	bool CreateLocalProfile(const RString& sName, RString& sProfileIDOut);
	void AddLocalProfileByID(
	  Profile* pProfile,
	  const RString& sProfileID); // transfers ownership of pProfile
	bool RenameLocalProfile(const RString& sProfileID, const RString& sNewName);
	bool DeleteLocalProfile(const RString& sProfileID);
	void GetLocalProfileIDs(vector<RString>& vsProfileIDsOut) const;
	void GetLocalProfileDisplayNames(
	  vector<RString>& vsProfileDisplayNamesOut) const;
	int GetLocalProfileIndexFromID(const RString& sProfileID) const;
	int GetNumLocalProfiles() const;

	RString GetStatsPrefix() { return m_stats_prefix; }
	void SetStatsPrefix(RString const& prefix);

	bool LoadFirstAvailableProfile(PlayerNumber pn, bool bLoadEdits = true);
	bool LoadLocalProfileFromMachine(PlayerNumber pn);
	bool SaveProfile(PlayerNumber pn) const;
	bool SaveLocalProfile(const RString& sProfileID);
	void UnloadProfile(PlayerNumber pn);

	void MergeLocalProfiles(RString const& from_id, RString const& to_id);
	void ChangeProfileType(int index, ProfileType new_type);
	void MoveProfilePriority(int index, bool up);

	// General data
	void IncrementToastiesCount(PlayerNumber pn);
	void AddStepTotals(PlayerNumber pn,
					   int iNumTapsAndHolds,
					   int iNumJumps,
					   int iNumHolds,
					   int iNumRolls,
					   int iNumMines,
					   int iNumHands,
					   int iNumLifts);

	bool IsPersistentProfile(PlayerNumber pn) const
	{
		return !m_sProfileDir.empty();
	}
	bool IsPersistentProfile(ProfileSlot slot) const;

	// return a profile even if !IsUsingProfile
	const Profile* GetProfile(PlayerNumber pn) const;
	Profile* GetProfile(PlayerNumber pn)
	{
		return (Profile*)((const ProfileManager*)this)->GetProfile(pn);
	}
	const Profile* GetProfile(ProfileSlot slot) const;
	Profile* GetProfile(ProfileSlot slot)
	{
		return (Profile*)((const ProfileManager*)this)->GetProfile(slot);
	}

	const RString& GetProfileDir(ProfileSlot slot) const;

	RString GetPlayerName(PlayerNumber pn) const;
	bool LastLoadWasTamperedOrCorrupt(PlayerNumber pn) const;
	bool LastLoadWasFromLastGood(PlayerNumber pn) const;

	// Song stats
	int GetSongNumTimesPlayed(const Song* pSong, ProfileSlot card) const;
	bool IsSongNew(const Song* pSong) const
	{
		return GetSongNumTimesPlayed(pSong, ProfileSlot_Player1) == 0;
	}
	void AddStepsScore(const Song* pSong,
					   const Steps* pSteps,
					   PlayerNumber pn,
					   const HighScore& hs,
					   int& iPersonalIndexOut,
					   int& iMachineIndexOut);
	void IncrementStepsPlayCount(const Song* pSong,
								 const Steps* pSteps,
								 PlayerNumber pn);
	// Lua
	void PushSelf(lua_State* L);

	static Preference1D<RString> m_sDefaultLocalProfileID;

  private:
	ProfileLoadResult LoadProfile(PlayerNumber pn, const RString& sProfileDir);

	// Directory that contains the profile.  Either on local machine or
	// on a memory card.
	RString m_sProfileDir;

	RString m_stats_prefix;
	Profile* dummy;
	bool m_bLastLoadWasTamperedOrCorrupt; // true if Stats.xml was
													   // present, but failed to
													   // load (probably because
													   // of a signature
													   // failure)
	bool m_bLastLoadWasFromLastGood; // if true, then
					 // m_bLastLoadWasTamperedOrCorrupt
					 // is also true
	mutable bool m_bNeedToBackUpLastLoad; // if true, back up
													   // profile on next save
	bool m_bNewProfile;
};

extern ProfileManager*
  PROFILEMAN; // global and accessible from anywhere in our program

#endif

/*
 * (c) 2003-2004 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
