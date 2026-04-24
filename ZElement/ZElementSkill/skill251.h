#ifndef __CPPGEN_GNET_SKILL251
#define __CPPGEN_GNET_SKILL251
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill251:public Skill
    {
      public:
        enum
        { SKILL_ID = 251 };
          Skill251 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill251Stub:public SkillStub
    {
      public:
        Skill251Stub ():SkillStub (251)
        {
            occupation = 1;
            name = L"ºñÍÁÖä";
            nativename = "ºñÍÁÖä";
            icon = "ºñÍÁÖä.dds";
            maxlevel = 15;
            maxlearn = 8;
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
            talent[0] = 653;
            talent[1] = 657;
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
        virtual ~ Skill251Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 244 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 20, 22, 24, 26, 28, 30, 32, 34 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel () + 2 * INT (skill->GetLevel () / 8));

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
            skill->GetPlayer ()->SetPasinchp (0.01001 * skill->GetLevel () + 0.02001 * INT (skill->GetLevel () / 8));
            skill->GetPlayer ()->SetPasinchpgen (0.05 * skill->GetT0 () * skill->GetLevel ());
            skill->GetPlayer ()->SetPasaddsmite (0.02 * skill->GetT1 () * skill->GetLevel ());
            skill->GetPlayer ()->SetPasaddhp (2 * skill->GetT0 () * skill->GetPlayer ()->GetLevel ());
            return true;
        }
#endif
    };
}
#endif
