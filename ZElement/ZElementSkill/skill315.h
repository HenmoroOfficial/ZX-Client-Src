#ifndef __CPPGEN_GNET_SKILL315
#define __CPPGEN_GNET_SKILL315
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill315:public Skill
    {
      public:
        enum
        { SKILL_ID = 315 };
          Skill315 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill315Stub:public SkillStub
    {
      public:
        Skill315Stub ():SkillStub (315)
        {
            occupation = 3;
            name = L"¾øÇéÖä";
            nativename = "¾øÇéÖä";
            icon = "¾øÇéÖä.dds";
            maxlevel = 12;
            maxlearn = 6;
            type = 11;
            eventflag = 1;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
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
            preskillnum = 1;
            talent[0] = 657;
            talent[1] = 658;
            talent_size = 2;
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill315Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 263 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 76, 78, 83, 88, 94, 97 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasaddhp (skill->GetLevel () * skill->GetPlayer ()->GetLevel () * (1 + 0.8 * skill->GetT1 ()));
            skill->GetPlayer ()->SetPasaddmp (skill->GetLevel () * skill->GetPlayer ()->GetLevel () * (1 + 0.8 * skill->GetT1 ()));
            skill->GetPlayer ()->SetPasaddsmite (0.02 * skill->GetT0 () * skill->GetLevel ());
            return true;
        }
#endif
    };
}
#endif
