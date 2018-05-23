//  Copyright (c) 2018, Marcin Drob

//  Kainote is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  Kainote is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with Kainote.  If not, see <http://www.gnu.org/licenses/>.

#include "HotkeysNaming.h"
#include "hotkeys.h"

HotkeysNaming::HotkeysNaming()
{
	CreateNamesMap();
}

HotkeysNaming::~HotkeysNaming()
{

}

const std::map<int, wxString> & HotkeysNaming::GetNamesTable()
{
	return names;
}

const wxString & HotkeysNaming::GetName(int id)
{
	auto & it = names.find(id);
	if (it != names.end())
		return it->second;

	return emptyString;
}

void HotkeysNaming::CreateNamesMap()
{
	names[About] = _("O programie");
	names[ANSI] = _("W�tek programu na forum AnimeSub.info");
	names[AspectRatio] = _("Zmie� proporcje wideo");
	names[ASSProperties] = _("W�a�ciwo�ci ASS");
	names[AudioCommit] = _("Zatwierd�");
	names[AudioCommitAlt] = _("Zatwierd� zast�pcze");
	names[AudioFromVideo] = _("Otw�rz audio z wideo");
	names[AudioPrevious] = _("Poprzednia linijka");
	names[AudioPreviousAlt] = _("Poprzednia linijka zast�pcze");
	names[AudioNext] = _("Nast�pna linijka");
	names[AudioNextAlt] = _("Nast�pna linijka zast�pcze");
	names[AudioPlay] = _("Odtwarzaj");
	names[AudioPlayAlt] = _("Odtwarzaj zast�pcze");
	names[AudioPlayLine] = _("Odtwarzaj lini�");
	names[AudioPlayLineAlt] = _("Odtwarzaj lini� zast�pcze");
	names[AudioStop] = _("Zatrzymaj");
	names[AudioGoto] = _("Przejd� do zaznaczenia");
	names[AudioScrollRight] = _("Przewi� w lewo");
	names[AudioScrollLeft] = _("Przewi� w prawo");
	names[AudioPlayBeforeMark] = _("Odtwarzaj przed znacznikem");
	names[AudioPlayAfterMark] = _("Odtwarzaj po znaczniku");
	names[AudioPlay500MSFirst] = _("Odtwarzaj pierwsze 500ms");
	names[AudioPlay500MSLast] = _("Odtwarzaj ko�cowe 500ms");
	names[AudioPlay500MSBefore] = _("Odtwarzaj 500ms przed");
	names[AudioPlay500MSAfter] = _("Odtwarzaj 500ms po");
	names[AudioPlayToEnd] = _("Odtwarzaj do ko�ca");
	names[AudioLeadin] = _("Dodaj wst�p");
	names[AudioLeadout] = _("Dodaj zako�czenie");
	names[AutoLoadScript] = _("Wczytaj skrypt");
	names[AutoReloadAutoload] = _("Od�wie� skrypty autoload");
	names[ChangeTime] = _("Okno zmiany czas�w");
	names[CloseAudio] = _("Zamknij audio");
	names[ContinousNext] = _("Ustaw czasy jako ci�g�e (nast�pna linijka)");
	names[ContinousPrevious] = _("Ustaw czasy jako ci�g�e (poprzednia linijka)");
	names[ConvertToASS] = _("Konwertuj do ASS");
	names[ConvertToSRT] = _("Konwertuj do SRT");
	names[ConvertToMDVD] = _("Konwertuj do MDVD");
	names[ConvertToMPL2] = _("Konwertuj do MPL2");
	names[ConvertToTMP] = _("Konwertuj do TMP");
	names[CopyCollumns] = _("Kopiuj kolumny");
	names[DeleteVideo] = _("Usu� plik wideo");
	names[Duplicate] = _("Duplikuj linie");
	names[EDITBOX_CHANGE_COLOR_OUTLINE] = _("Kolor obw�dki");
	names[EDITBOX_CHANGE_COLOR_PRIMARY] = _("Kolor podstawowy");
	names[EDITBOX_CHANGE_COLOR_SECONDARY] = _("Kolor zast�pczy do karaoke");
	names[EDITBOX_CHANGE_COLOR_SHADOW] = _("Kolor cienia");
	names[EDITBOX_CHANGE_UNDERLINE] = _("Wyb�r czcionki");
	names[EDITBOX_COMMIT] = _("Zatwierd� zmiany");
	names[EDITBOX_COMMIT_GO_NEXT_LINE] = _("Zatwierd� zmiany id� do nast�pnej linii");
	names[EDITBOX_HIDE_ORIGINAL] = _("Ukryj orygina�");
	names[EDITBOX_PASTE_ALL_TO_TRANSLATION] = _("Wklej wszystko");
	names[EDITBOX_PASTE_SELECTION_TO_TRANSLATION] = _("Wklej zaznaczone");
	names[EDITBOX_TAG_BUTTON1] = _("Pierwszy przycisk tag�w");
	names[EDITBOX_TAG_BUTTON2] = _("Drugi przycisk tag�w");
	names[EDITBOX_TAG_BUTTON3] = _("Trzeci przycisk tag�w");
	names[EDITBOX_TAG_BUTTON4] = _("Czwarty przycisk tag�w");
	names[EDITBOX_TAG_BUTTON5] = _("Pi�ty przycisk tag�w");
	names[EDITBOX_TAG_BUTTON6] = _("Sz�sty przycisk tag�w");
	names[EDITBOX_TAG_BUTTON7] = _("Si�dmy przycisk tag�w");
	names[EDITBOX_TAG_BUTTON8] = _("�smy przycisk tag�w");
	names[EDITBOX_TAG_BUTTON9] = _("Dziewi�ty przycisk tag�w");
	names[EDITBOX_TAG_BUTTON10] = _("Dziesi�ty przycisk tag�w");
	names[Editor] = _("W��cz / Wy��cz edytor");
	names[EndDifference] = _("Wstaw r�nic� ko�cow�");
	names[FilterByDialogues] = _("Ukryj komentarze");
	names[FilterByDoubtful] = _("Poka� niepewne");
	names[FilterByNothing] = _("Wy��cz filtrowanie");
	names[FilterBySelections] = _("Ukryj zaznaczone linie");
	names[FilterByStyles] = _("Ukryj linie ze stylami");
	names[FilterByUntranslated] = _("Poka� nieprzet�umaczone");
	names[FindNextDoubtful] = _("Nast�pne niepewne");
	names[FindNextUntranslated] = _("Nast�pne nieprzet�umaczone");
	names[FindReplaceDialog] = _("Znajd� i zmie�");
	names[FontCollectorID] = _("Kolekcjoner czcionek");
	names[FPSFromVideo] = _("Ustaw FPS z wideo");
	names[FrameToClipboard] = _("Kopiuj klatk� do schowka");
	names[FrameToPNG] = _("Zapisz klatk� jako PNG");
	names[GLOBAL_SORT_ALL_BY_START_TIMES] = _("Sortuj wszystko wed�ug czasu pocz�tkowego");
	names[GLOBAL_SORT_ALL_BY_END_TIMES] = _("Sortuj wszystko wed�ug czasu ko�cowego");
	names[GLOBAL_SORT_ALL_BY_STYLE] = _("Sortuj wszystko wed�ug styl�w");
	names[GLOBAL_SORT_ALL_BY_ACTOR] = _("Sortuj wszystko wed�ug aktora");
	names[GLOBAL_SORT_ALL_BY_EFFECT] = _("Sortuj wszystko wed�ug efektu");
	names[GLOBAL_SORT_ALL_BY_LAYER] = _("Sortuj wszystko wed�ug warstwa");
	names[GLOBAL_SORT_SELECTED_BY_START_TIMES] = _("Sortuj wszystko wed�ug czasu pocz�tkowego");
	names[GLOBAL_SORT_SELECTED_BY_END_TIMES] = _("Sortuj wszystko wed�ug czasu ko�cowego");
	names[GLOBAL_SORT_SELECTED_BY_STYLE] = _("Sortuj wszystko wed�ug styl�w");
	names[GLOBAL_SORT_SELECTED_BY_ACTOR] = _("Sortuj wszystko wed�ug aktora");
	names[GLOBAL_SORT_SELECTED_BY_EFFECT] = _("Sortuj wszystko wed�ug efektu");
	names[GLOBAL_SORT_SELECTED_BY_LAYER] = _("Sortuj wszystko wed�ug warstwa");
	names[GLOBAL_SHIFT_TIMES] = _("Przesu� czasy / uruchom post processor");
	names[GoToPrewKeyframe] = _("Przejd� do poprzedniej klatki kluczowej");
	names[GoToNextKeyframe] = _("Przejd� do nast�pnej klatki kluczowej");
	names[GRID_FILTER] = _("Filtruj");
	names[GRID_FILTER_AFTER_SUBS_LOAD] = _("Filtruj po wczytaniu napis�w");
	names[GRID_FILTER_DO_NOT_RESET] = _("Nie resetuj wcze�niejszego filtrowania");
	names[GRID_FILTER_IGNORE_IN_ACTIONS] = _("Ignoruj filtrowanie przy akcjach");
	names[GRID_FILTER_INVERTED] = _("Filtrowanie odwr�cone");
	names[GRID_HIDE_ACTOR] = _("Ukryj aktora");
	names[GRID_HIDE_CPS] = _("Ukryj znaki na sekund�");
	names[GRID_HIDE_END] = _("Ukryj czas ko�cowy");
	names[GRID_HIDE_EFFECT] = _("Ukryj efekt");
	names[GRID_HIDE_LAYER] = _("Ukryj warstw�");
	names[GRID_HIDE_MARGINL] = _("Ukryj lewy margines");
	names[GRID_HIDE_MARGINR] = _("Ukryj prawy margines");
	names[GRID_HIDE_MARGINV] = _("Ukryj pionowy margines");
	names[GRID_HIDE_START] = _("Ukryj czas pocz�tkowy");
	names[GRID_HIDE_STYLE] = _("Ukryj styl");
	names[Help] = _("Pomoc (niekompletna, ale jednak)");
	names[Helpers] = _("&Lista os�b pomocnych przy tworzeniu programu");
	names[HideProgressBar] = _("Ukryj / poka� pasek post�pu");
	names[HideSelected] = _("Ukryj zaznaczone linijki");
	names[HideTags] = _("Ukryj tagi w nawiasach");
	names[History] = _("Historia");
	names[InsertAfter] = _("Wstaw p&o");
	names[InsertAfterVideo] = _("Wstaw po z c&zasem wideo");
	names[InsertAfterWithVideoFrame] = _("Wstaw po z czasem klatki wideo");
	names[InsertBefore] = _("Wstaw &przed");
	names[InsertBeforeVideo] = _("Wstaw przed z &czasem wideo");
	names[InsertBeforeWithVideoFrame] = _("Wstaw przed z czasem klatki wideo");
	names[Join] = _("Z��cz &linijki");
	names[JoinToFirst] = _("Z��cz linijki zostaw pierwsz�");
	names[JoinToLast] = _("Z��cz linijki zostaw ostatni�");
	names[JoinWithPrevious] = _("Scal z poprzedni� linijk�");
	names[JoinWithNext] = _("Scal z nast�pn� linijk�");
	names[LoadLastScript] = _("Uruchom ostatnio zaczytany skrypt");
	names[Minus5Second] = _("5 sekund do ty�u");
	names[Minus5SecondG] = _("Wideo minus 5 sekund");
	names[MinusMinute] = _("Minuta do ty�u");
	names[NewFPS] = _("Ustaw nowy FPS");
	names[NextChapter] = _("Nast�pny rozdzia�");
	names[NextFrame] = _("Klatka w prz�d");
	names[NextLine] = _("Nast�pna linijka");
	names[NextTab] = _("Nast�pna karta");
	names[NextVideo] = _("Nast�pny plik");
	names[OpenAudio] = _("Otw�rz audio");
	names[OpenSubs] = _("Otw�rz napisy");
	names[OpenVideo] = _("Otw�rz wideo");
	names[PasteCollumns] = _("Wklej kolumny");
	names[PasteTranslation] = _("Wklej tekst t�umaczenia");
	names[PlayActualLine] = _("Odtw�rz aktywn� linijk�");
	names[PlayPause] = _("Odtw�rz / Pauza");
	names[PlayPauseG] = _("Odtw�rz / Pauza");
	names[Plus5SecondG] = _("Wideo plus 5 sekund");
	names[Plus5Second] = _("5 sekund do przodu");
	names[PlusMinute] = _("Minuta do przodu");
	names[PreviousChapter] = _("Poprzedni rozdzia�");
	names[PreviousFrame] = _("Klatka w ty�");
	names[PreviousLine] = _("Poprzednia linijka");
	names[PreviousTab] = _("Poprzednia karta");
	names[PreviousVideo] = _("Poprzedni plik");
	names[PutBold] = _("Wstaw pogrubienie");
	names[PutItalic] = _("Wstaw kursyw�");
	names[Quit] = _("Wyj�cie");
	names[Redo] = _("Pon�w");
	names[Remove] = _("Usu� linijk�");
	names[RemoveSubs] = _("Usu� napisy z e&dytora");
	names[RemoveText] = _("Usu� tekst");
	names[SaveAllSubs] = _("Zapisz wszystkie napisy");
	names[SaveSubs] = _("Zapisz");
	names[SaveSubsAs] = _("Zapisz jako..."); 
	names[SaveTranslation] = _("Zapisz &t�umaczenie");
	names[Search] = _("Znajd�");
	names[SelectFromVideo] = _("Zaznacz lini� z czasem wideo");
	names[SelectLinesDialog] = _("Zaznacz linijki");
	names[SetAudioFromVideo] = _("Ustaw audio z czasem wideo");
	names[SetAudioMarkFromVideo] = _("Ustaw znacznik audio z czasem wideo");
	names[SetDoubtful] = _("Ustaw jako niepewne i przejd� dalej");
	names[SetEndTime] = _("Wstaw czas ko�cowy z wideo");
	names[SetStartTime] = _("Wstaw czas pocz�tkowy z wideo"); 
	names[Settings] = _("&Ustawienia");
	names[SetVideoAtStart] = _("Przejd� do czasu pocz�tkowego linii");
	names[SetVideoAtEnd] = _("Przejd� do czasu ko�cowego linii");
	names[ShowPreview] = _("Poka� podgl�d napis�w");
	names[SnapWithEnd] = _("Przyklej koniec do klatki kluczowej");
	names[SnapWithStart] = _("Przyklej start do klatki kluczowej");
	names[SpellcheckerDialog] = _("Sprawd� poprawno�� pisowni");
	names[SplitLine] = _("Wstaw znak podzia�u");
	names[StartDifference] = _("Wstaw r�nic� pocz�tkow�");
	names[StopPlayback] = _("Zatrzymaj");
	names[StyleManager] = _("Mened�er styl�w");
	names[SubbedFrameToClipboard] = _("Kopiuj klatk� z napisami do schowka");
	names[SubbedFrameToPNG] = _("Zapisz klatk� z napisami jako PNG");
	names[SubsResample] = _("Zmie� rozdzielczo�� napis�w");
	names[SubsFromMKV] = _("Wczytaj napisy z pliku MKV");
	names[Swap] = _("Za&mie�");
	names[TranslationDialog] = _("Okno przesuwania dialog�w");
	names[Undo] = _("Cofnij");
	names[UndoToLastSave] = _("Cofnij do ostatniego zapisu");
	names[VideoZoom] = _("Powi�ksz wideo");
	names[ViewAll] = _("Widok wszystko");
	names[ViewAudio] = _("Widok audio i napisy");
	names[ViewSubs] = _("Widok tylko napisy");
	names[ViewVideo] = _("Widok wideo i napisy");
	names[VolumePlus] = _("D�wi�k g�o�niej"); 
	names[VolumeMinus] = _("D�wi�k ciszej"); 
	
}
