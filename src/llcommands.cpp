//#include "StdAfx.h"

#include "../include/llcommands.h"
//#include "../include/llmaplist.h"

#ifndef _MSC_VER
#include "../include/def.h"
#else
#include <windows.h>
//#define USE_CATCH 
#endif


void llCommands::SetDefaults() {
	mesg     = _llLogger();
	utils    = _llUtils();
	logfile  = NULL;
	skip_next_block = 0;
	lines.resize(0);
	worker_cache.resize(0);
	worker_flags.resize(0);
	worker_list.resize(0);
	sections.resize(0);
	CurrentCommand = "";
	worker_pointer = 0;
	line_pointer   = 0;
	block_level    = 0;
	noworkerprint  = 0;
}

//constructor
llCommands::llCommands(FILE *_file, const char *_section) {
	SetDefaults();
	section  = _section;
	file     = _file;
	filename = NULL;
}

//constructor
llCommands::llCommands(const char *_file, const char *_section) {
	SetDefaults();
	section  = _section;
	filename = new char[strlen(_file)];
	strcpy_s(filename, strlen(_file) + 1, _file);
	if (fopen_s(&file, filename, "r")) {
		return;
	}
}

llCommands::llCommands() {
	SetDefaults();
	section  = "[None]";
	filename = NULL;
	file     = NULL;
}

int llCommands::Open(const char *_file, const char *_section) {
	section = _section;

	if (filename) delete filename;

	filename = new char[strlen(_file) + 1];
	strcpy_s(filename, strlen(_file) + 1, _file);
		
	if (file && file != stdin) {
		fclose(file);
		file = NULL;
	}
	
	if (fopen_s(&file, filename, "r")) {
		mesg->WriteNextLine(LOG_ERROR, "Unable to open %s", filename);
		file = NULL;
		return 0;
	}

	return 1;
}

int llCommands::ReadCache(int no_end) {

	if (!file) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "llCommands::ReadCache: no file open");
		return 0;
	}
	
	while ((fgets(dummyline, LLCOM_MAX_LINE, file)) && ((_strnicmp(dummyline, "@end", 3) != 0) || no_end )) {
		for (unsigned int i=0; i<strlen(dummyline); i++) { //covert DOS files
			if (dummyline[i]=='\r' || dummyline[i]=='\n') dummyline[i]='\0';
		}
		int num = lines.size();
		lines.resize(num+1);
		lines[num] = new char[strlen(dummyline)+1];
		strcpy_s(lines[num], strlen(dummyline)+1, dummyline);
	}
	
	fclose(file);
	file = NULL;
	return 1;
};

int llCommands::SaveFile(const char *_file) {
	FILE * wfile = NULL;
	if (fopen_s(&wfile, _file, "w")) {
		mesg->WriteNextLine(LOG_ERROR, "Unable to open %s", _file);
		return 0;
	}

	if (_llUtils()->GetValue("_gamemode")) {
		fprintf(wfile, "[_gamemode]\n");
		fprintf(wfile, "%s -name=\"%s\"\n", LLCOM_GAMEMODE_CMD, _llUtils()->GetValue("_gamemode"));
	}

	int save=1;
	for (unsigned int i=0; i<lines.size(); i++) {
		//Filter hidden sections:
		if (strlen(lines[i])>1 && _strnicmp(lines[i], "[_",2)==0)
			save = 0;
		else if (strlen(lines[i])>0 && _strnicmp(lines[i],"[",1)==0)
			save = 1;
		if (save)
			fprintf(wfile,"%s\n", lines[i]);
	}

	//save all variables which are not hidden:
	fprintf(wfile, "[_saved]\n");
	utils->WriteFlags(wfile);

	fclose(wfile);
	return 1;
};

int llCommands::ReadStdin(void) {

	while (gets_s(dummyline, LLCOM_MAX_LINE)) {
		int num = lines.size();
		lines.resize(num+1);
		lines[num] = new char[strlen(dummyline)+1];
		strcpy_s(lines[num], strlen(dummyline)+1, dummyline);
	}

	std::cout << lines.size() << std::endl;

	return 1;
};

char *llCommands::GetNextLine(int _cmd) {
	if (_cmd < 0) {
		current_dump_line = 0;
		return NULL;
	}
	if (current_dump_line >= lines.size()) return NULL;
	current_dump_line++;
	return lines[current_dump_line - 1];
};

int llCommands::Close() {
	section = NULL;

	worker_pointer = 0;

	for (unsigned int i=0; i<worker_cache.size(); i++) {
		if (worker_cache[i] != NULL) delete worker_cache[i];
	}
	worker_cache.resize(0);

	if (lines.size()) {
		line_pointer = 0;
		lines.resize(0);
	}

	worker_flags.resize(0);
	sections.resize(0);
	section_cache.resize(0);

	line_pointer   = 0;
	block_level    = 0;

	if (filename) filename = NULL;
	if (file) {
		fclose(file);
		file = NULL;
	}

	return 1;
}

int llCommands::CompileScript(int _compile_all_sections) {
	char *current_section = "";
	
	for (unsigned int l=0; l<lines.size(); l++) {
		strcpy_s(dummyline, LLCOM_MAX_LINE, lines[l]);
		
		char *linex = dummyline;
		_llUtils()->StripSpaces(&linex);
		_llUtils()->StripComment(linex);
		_llUtils()->StripSpaces(&linex);
		
		//_llLogger()->WriteNextLine(LOG_INFO, "linex %s", linex);

		if (strlen(linex) > 0) {
			if (linex[0] == '[') {
				//avoid re-compiling of an existing section:
				int is_existing = 0;
				if (_compile_all_sections) { //dont care
					is_existing = 1;
				} else {
					if (_stricmp(linex, section) != 0) { //never create section if section is not used
						//_llLogger()->WriteNextLine(LOG_INFO, "sec skip %s", linex);
						is_existing = 1;
					} else {
						//_llLogger()->WriteNextLine(LOG_INFO, "sec todo %s", linex);
						for (unsigned int i=0; i<sections.size(); i++) {
							if (_stricmp(linex, sections[i]) == 0) is_existing = 1;
						}
					}
				}
				if (!is_existing) {
					char *sec = _llUtils()->NewString(linex);
					sections.push_back(sec);
					current_section = sec;
					//_llLogger()->WriteNextLine(LOG_INFO, "sec current %s", current_section);
				} else {
					current_section = "delme";
				}
			} else if (_compile_all_sections || _stricmp(current_section, section) == 0) {
				ExtendWorkerCache();

				//_llLogger()->WriteNextLine(LOG_INFO, "sec comp %s", section);
				
				//check for flags
repeat:				
				if (linex[0] == '@') {
					unsigned int i = _llUtils()->SeekNextSpace(linex);
					if (!i) i = strlen(linex);

					linex[i] = '\0';
					char *delme = _llUtils()->NewString(linex);
					
					(worker_flags.back()).push_back(delme);

					linex = linex + i + 1;
					_llUtils()->StripSpaces(&linex);
					goto repeat;
				}

				_llUtils()->StripSpaces(&linex);
				if (linex[0] == '{') {
					ExtendWorkerCache();
					bracket_cache[bracket_cache.size()-1] = LLCOM_OPEN_BLOCK;
					linex++;
					_llUtils()->StripSpaces(&linex);
					ExtendWorkerCache();
				} else if (linex[0] == '}') {
					ExtendWorkerCache();
					bracket_cache[bracket_cache.size()-1] = LLCOM_CLOSE_BLOCK;
					linex++;
					_llUtils()->StripSpaces(&linex);	
					ExtendWorkerCache();
				} 

				int com = -1;
				char *ptr, *ptr2;
				char *saveptr1 = NULL, 
					*saveptr2 = NULL;
				ptr = strtok_int(linex, ' ', &saveptr1);
				if (ptr && strlen(ptr)>0) {

					if (_stricmp(ptr, "{") == 0) {
						ExtendWorkerCache();
						bracket_cache[bracket_cache.size()-1] = LLCOM_OPEN_BLOCK;
						if (saveptr1) linex = saveptr1;
						else linex = (char *)"";
						_llUtils()->StripSpaces(&linex);
						ExtendWorkerCache();
						goto repeat;
					}
					if (_stricmp(ptr, "}") == 0) {
						ExtendWorkerCache();
						bracket_cache[bracket_cache.size()-1] = LLCOM_CLOSE_BLOCK;
						if (saveptr1) linex = saveptr1;
						else linex = (char *)"";
						_llUtils()->StripSpaces(&linex);	
						ExtendWorkerCache();
						goto repeat;
					}

					llWorker *worker = NULL;

					for (unsigned int i=0; i<worker_list.size(); i++) {
						if (_stricmp(ptr, worker_list[i]->GetCommandName() ) == 0) {
							com            = i;
							CurrentCommand = worker_list[i]->GetCommandName();
							worker         = worker_list[i]->Clone();
							worker                         ->SetCommandIndex(com);
							worker                         ->RegisterOptions();
							worker                         ->Prepare();
							worker_cache[worker_cache.size()-1] = worker;

							ptr = strtok_int(NULL,' ', &saveptr1);
							while(ptr != NULL) {

								if (_stricmp(ptr, "{") == 0) {
									ExtendWorkerCache();
									bracket_cache[bracket_cache.size()-1] = LLCOM_OPEN_BLOCK;
									if (saveptr1) linex = saveptr1;
									else linex = (char *)"";
									_llUtils()->StripSpaces(&linex);
									ExtendWorkerCache();
									goto repeat;
								}
								if (_stricmp(ptr, "}") == 0) {
									ExtendWorkerCache();
									bracket_cache[bracket_cache.size()-1] = LLCOM_CLOSE_BLOCK;
									if (saveptr1) linex = saveptr1;
									else linex = (char *)"";
									_llUtils()->StripSpaces(&linex);	
									ExtendWorkerCache();
									goto repeat;
								}

								if (!worker->CheckFlag(ptr)) {
									ptr2 = strtok_int(ptr, '=', &saveptr2);
									if (ptr2!=NULL && strlen(ptr2)>0) {
										if (worker->CheckValue(ptr)) {
											ptr2 = strtok_int(NULL, '=', &saveptr2);
											if (ptr2)
												worker->AddValue(ptr2);
											else {
												mesg->WriteNextLine(-LOG_ERROR, "Compile error in line %i", l+1);
												mesg->WriteNextLine(-LOG_ERROR, LLCOM_SYNTAX_ERROR, ptr, CurrentCommand);
											}
										} else {
											mesg->WriteNextLine(-LOG_ERROR, "Compile error in line %i", l+1);
											mesg->WriteNextLine(-LOG_ERROR, LLCOM_UNKNOWN_OPTION, ptr, CurrentCommand);
										}
									}
								}
								ptr = strtok_int(NULL, ' ', &saveptr1);
							}
							goto exit;
						} //if stricmp
					}
exit:
					if (!worker) {
						mesg->WriteNextLine(-LOG_ERROR, "Compile error in line %i", l+1);
						mesg->WriteNextLine(-LOG_ERROR, "--> Unknown command [%s]", ptr);						
					}
				} //strlen ptr
			} //not section
		} //if (strlen(linex) > 0) 
	}

	return 1;
}

int llCommands::GetCommand(void) {

	CurrentCommand = NULL;

	if (worker_pointer == worker_cache.size()) {
		return -2; //EOF
	}

	worker_pointer++;

	if (section) {
		if (section_cache[worker_pointer-1] < 0)
			return 0; //no section stored
		if (_stricmp(sections[section_cache[worker_pointer-1]], section)==0) {
			section_is_good = 1;
		} else 
			section_is_good = 0;
		if (!section_is_good) 
			return 0;
	}

	//loop over flags
	unsigned int flagsize = (worker_flags[worker_pointer-1]).size();
	for (unsigned int i=0; i<flagsize; i++) {
		int negative = 0;
		char *flagline = (worker_flags[worker_pointer-1])[i];

		if (strlen(flagline) > 1 && flagline[1] == '!') 
			negative = 1;

		int found = (utils->IsEnabled(flagline+1+negative) == 1 ? 1: 0);

		if (!found && negative==0) {
			skip_next_block = LLCOM_SKIP_BLOCK;
		}
		if (found && negative==1) {
			skip_next_block = LLCOM_SKIP_BLOCK;
		}
	}

	//check for block
	if (bracket_cache[worker_pointer-1] == LLCOM_OPEN_BLOCK) {
		if (block_level == LLCOM_MAX_NESTED_BLOCKS) {
			mesg->WriteNextLine(-LOG_FATAL, "Too many nested blocks (max=%i)", LLCOM_MAX_NESTED_BLOCKS);
		}
		block_skip[block_level] = skip_next_block;
		bracket_reference[block_level] = worker_pointer - 2;
		if (block_level) block_skip[block_level] |= (block_skip[block_level-1] & LLCOM_SKIP_BLOCK);
		skip_next_block = 0;
		block_level++;
		return 0;
	}
	if (bracket_cache[worker_pointer-1] == LLCOM_CLOSE_BLOCK) {
		if (block_level == 0) {
			mesg->WriteNextLine(-LOG_FATAL, "Too many '}': nothing to close here...");
		}
		skip_next_block = 0;
		block_level--;
		if (block_skip[block_level] & LLCOM_ITERATE_BLOCK) {
			if (bracket_reference[block_level] < 0) {
				mesg->WriteNextLine(-LOG_FATAL, "No reference found for closing '}'");
			}
			worker_pointer = bracket_reference[block_level];
		}
		return 0;
	}
	
	llWorker *worker = worker_cache[worker_pointer - 1];
	if (skip_next_block == LLCOM_SKIP_BLOCK || (block_level && block_skip[block_level-1]==LLCOM_SKIP_BLOCK)) {
		if (worker) {
			skip_next_block = 0;
			//std::cout << "skipped:" << std::endl;
			//worker->ReplaceFlags();
			//worker->Print();
		}	
		return 0;
	}
	
	int com = -1;

	if (worker) {
		if (skip_next_block & LLCOM_ITERATE_BLOCK) worker_pointer -= 2;
		skip_next_block = 0;
		com = worker->GetCommandIndex();
		CurrentCommand = worker->GetCommandName();
		worker->Prepare();
	} else return 0;
	
	//afterburner	
	worker->ReplaceFlags();
	if (!noworkerprint) worker->Print();
	
	if (worker->IsSilent()) _llLogger()->level=LOG_WARNING;
	int retval = worker->Exec();
	if (worker->IsSilent()) _llLogger()->level=LOG_INFO;
	
	if (worker->Repeat()) {
		skip_next_block |= LLCOM_ITERATE_BLOCK;
	} else if (worker->IsRepeatWorker() && !retval) {
		skip_next_block |= LLCOM_SKIP_BLOCK;
	}

	if (retval == -1) return -2;

	return com;
}

int llCommands::Loop(void) {

#ifdef _MSC_VER
	__int64 time_statistics[LLCOM_MAX_WORKERS];
	int time_statistics_cmd[LLCOM_MAX_WORKERS];
	char *time_statistics_cmdname[LLCOM_MAX_WORKERS];
	unsigned int time_statistics_pointer = 0;
#endif

	//******************
	//batch loop
	//******************

	int com = 0;

	mesg->WriteNextLine(LOG_INFO, "****** Go into batch mode in %s ******", section);

	while (com > -2) {

#ifdef _MSC_VER
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		BOOL res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
#endif

#ifdef USE_CATCH
		try {
#endif
			com = GetCommand();

#ifdef USE_CATCH
		} catch (char *str) {
			if (CurrentCommand)
				mesg->WriteNextLine(-LOG_FATAL, "Catched exception [%s] in [%s]", str, CurrentCommand);
			else 
				mesg->WriteNextLine(-LOG_FATAL, "Catched exception [%s]", str);
		} catch (int str) {
			if (CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL, "Catched exception [%i] in [%s]", str, CurrentCommand);
			else
				mesg->WriteNextLine(LOG_FATAL, "Catched exception [%i]", str);
		} catch (...) {
			if (CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL, "Catched exception in [%s]", CurrentCommand);
			else
				mesg->WriteNextLine(LOG_FATAL, "Catched exception");
		}
#endif

		mesg->Dump();

#ifdef _MSC_VER
		FILETIME userTime_old = userTime;

		res = GetSystemTimes( &idleTime, &kernelTime, &userTime );

		BOOL found = false;
		for (unsigned int ii=0; ii<time_statistics_pointer; ii++) {
			if (com == time_statistics_cmd[ii]) {
				ULARGE_INTEGER u1 = { userTime.dwLowDateTime, userTime.dwHighDateTime }; 
				ULARGE_INTEGER u2 = { userTime_old.dwLowDateTime, userTime_old.dwHighDateTime }; 
				time_statistics[ii] += u1.QuadPart - u2.QuadPart;
				found = true;
			}
		}
		if (!found && CurrentCommand) {
			ULARGE_INTEGER u1 = { userTime.dwLowDateTime, userTime.dwHighDateTime }; 
			ULARGE_INTEGER u2 = { userTime_old.dwLowDateTime, userTime_old.dwHighDateTime }; 
			time_statistics[time_statistics_pointer] = u1.QuadPart - u2.QuadPart;
			time_statistics_cmd[time_statistics_pointer] = com;
			time_statistics_cmdname[time_statistics_pointer] = _llUtils()->NewString((char*) CurrentCommand);
			time_statistics_pointer++;
		}
#endif

	}

	std::cout << "****** Batch loop done ******" << std::endl;

#ifdef _MSC_VER
	for (unsigned int ii=0; ii<time_statistics_pointer; ii++) {
		for (unsigned int jj=0; jj<time_statistics_pointer-1; jj++) {
			if (time_statistics[jj] < time_statistics[jj+1]) {
				__int64 time_statistics_tmp = time_statistics[jj];
				char * time_statistics_cmdname_tmp = time_statistics_cmdname[jj];
				time_statistics[jj] = time_statistics[jj+1];
				time_statistics_cmdname[jj] = time_statistics_cmdname[jj+1];
				time_statistics[jj+1] = time_statistics_tmp;
				time_statistics_cmdname[jj+1] = time_statistics_cmdname_tmp;
			}
		}
	}

	std::cout << "User time per command (sorted):" << std::endl;
	for (unsigned int ii=0; ii<time_statistics_pointer; ii++) {
		if (time_statistics[ii] > 1000000)
			std::cout << time_statistics_cmdname[ii] << ": " << (((double)time_statistics[ii]) /10000000.)<< " s" << std::endl;
	}
#endif

	return 0;

}