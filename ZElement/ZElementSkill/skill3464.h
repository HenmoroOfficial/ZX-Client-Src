#ifndef __CPPGEN_GNET_SKILL3464
#define __CPPGEN_GNET_SKILL3464
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3464:public Skill
    {
      public:
        enum
        { SKILL_ID = 3464 };
          Skill3464 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3464Stub:public SkillStub
    {
      public:
        Skill3464Stub ():SkillStub (3464)
        {
            occupation = 96;
            name = L"½Ã½¡";
            nativename = "½Ã½¡";
            icon = "½Ã½¡.dds";
            maxlevel = 9;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 10;
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
            talent[0] = 3535;
            talent[1] = 3530;
            talent_size = 2;
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill3464Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3455 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 5, 7, 9, 11, 13, 15 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel (), 20 * skill->GetLevel ());

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
            skill->GetPlayer ()->SetPasaddhp (skill->GetPlayer ()->GetLevel () * (skill->GetLevel () + skill->GetT1 () * 2));
            skill->GetPlayer ()->SetPasinchpgen (0.2 * skill->GetLevel ());
            skill->GetPlayer ()->SetPasaddsilent (skill->GetT0 ());
            skill->GetPlayer ()->SetPasaddweak (skill->GetT0 ());
            skill->GetPlayer ()->SetPasadddizzy (skill->GetT0 ());
            skill->GetPlayer ()->SetPasaddsleep (skill->GetT0 ());
            skill->GetPlayer ()->SetPasaddwrap (skill->GetT0 ());
            skill->GetPlayer ()->SetPasaddsmite (skill->GetLevel () * skill->GetT0 () * 0.015);
            return true;
        }
#endif
    };
}
#endif
