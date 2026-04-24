#ifndef __CPPGEN_GNET_SKILL1572
#define __CPPGEN_GNET_SKILL1572
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1572:public Skill
    {
      public:
        enum
        { SKILL_ID = 1572 };
          Skill1572 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1572Stub:public SkillStub
    {
      public:
        Skill1572Stub ():SkillStub (1572)
        {
            occupation = 150;
            name = L"ð§ÒÀÕæ·¨";
            nativename = "ð§ÒÀÕæ·¨";
            icon = "ð§ÒÀÕæ·¨.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 11;
            eventflag = 1;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 4;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            talent[0] = 1574;
            talent[1] = 1576;
            talent[2] = 1578;
            talent[3] = 1580;
            talent_size = 4;
            action[0] = "ÇàÔÆÃÅ_ð§ÒÀÕæ·¨";
            action[1] = "ÇàÔÆÃÅ_ð§ÒÀÕæ·¨";
            action[2] = "¹íÍõ×Ú_ð§ÒÀÕæ·¨";
            action[3] = "ºÏ»¶ÅÉ_ð§ÒÀÕæ·¨";
            action[4] = "ÌìÒôËÂ_ð§ÒÀÕæ·¨";
            action[5] = "¹íµÀ_ð§ÒÀÕæ·¨";
            action[6] = "·ÙÏã_ð§ÒÀÕæ·¨";
            action[7] = "ÊÞÉñ_ð§ÒÀÕæ·¨";
            action[8] = "ºüÑý_ð§ÒÀÕæ·¨";
            action[9] = "ÐùÔ¯_ð§ÒÀÕæ·¨";
            action[10] = "»³¹â_ð§ÒÀÕæ·¨";
            action[11] = "Ì«ê»_ð§ÒÀÕæ·¨";
            action[12] = "Ìì»ª_ð§ÒÀÕæ·¨";
            action[13] = "ÁéÙí_ð§ÒÀÕæ·¨";
            action[14] = "Ó¢ÕÐ_ð§ÒÀÕæ·¨";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ð§ÒÀÕæ·¨";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[20] = "¹íµÀ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[21] = "·ÙÏã_Æï³Ë_ð§ÒÀÕæ·¨";
            action[22] = "ÊÞÉñ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[23] = "ºüÑý_Æï³Ë_ð§ÒÀÕæ·¨";
            action[24] = "ÐùÔ¯_Æï³Ë_ð§ÒÀÕæ·¨";
            action[25] = "»³¹â_Æï³Ë_ð§ÒÀÕæ·¨";
            action[26] = "Ì«ê»_Æï³Ë_ð§ÒÀÕæ·¨";
            action[27] = "Ìì»ª_Æï³Ë_ð§ÒÀÕæ·¨";
            action[28] = "ÁéÙí_Æï³Ë_ð§ÒÀÕæ·¨";
            action[29] = "Ó¢ÕÐ_Æï³Ë_ð§ÒÀÕæ·¨";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
#endif
        }
        virtual ~ Skill1572Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 90, 90, 95, 100, 105, 110, 115, 120, 125 };
            return array[level - 1];
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 30000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               90 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel (), 4 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasdecfatalratio (int (skill->GetLevel () / 10) *
                                                      (int (skill->GetT0 () / 10) + int (skill->GetT1 () / 10) + int (skill->GetT2 () / 10) +
                                                       int (skill->GetT3 () / 10)) *0.01);
              skill->GetPlayer ()->SetPasaddhp (90 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ());
              skill->GetPlayer ()->SetPasadddizzy (4 * skill->GetLevel ());
              skill->GetPlayer ()->SetPasincskilldodge (50 * norm (skill->GetLevel () / 9.9));
              return true;
        }
#endif
    };
}
#endif
