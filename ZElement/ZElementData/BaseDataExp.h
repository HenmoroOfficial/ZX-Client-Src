#ifndef BASEDATAEXP_H_
#define BASEDATAEXP_H_

void ConvertMultilineText(char* szText, bool bToMultiline);
bool ExportAllBaseData(const char* szPath);
bool ExportPolicyData( const char* szPath);
bool CheckMd5CodeForTemplate(char *szPathName);
void TempTransVersion();

#endif