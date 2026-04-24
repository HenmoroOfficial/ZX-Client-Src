#ifndef __CPPGEN_GNET_SKILL3440
#define __CPPGEN_GNET_SKILL3440
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3440:public Skill
    {
      public:
        enum
        { SKILL_ID = 3440 };
          Skill3440 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3440Stub:public SkillStub
    {
      public:
        Skill3440Stub ():SkillStub (3440)
        {
            occupation = 66;
            name = L"天宗心法";
            nativename = "天宗心法";
            icon = "天宗心法.dds";
            maxlevel = 20;
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
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
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
            talent[0] = 3417;
            talent[1] = 3407;
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
            action[30] = "";
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
        virtual ~ Skill3440Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3439 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 85, 87, 89, 91, 93, 95, 97, 99 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 8 * skill->GetLevel (), 6 * skill->GetLevel ());

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
            skill->GetPlayer ()->SetPasadddodge (6 * skill->GetLevel ());
            skill->GetPlayer ()->SetPasaddaccuracy (8 * skill->GetLevel ());
            skill->GetPlayer ()->SetPasincskilldodge (int (skill->GetLevel () / 4) * norm (skill->GetPlayer ()->GetReborncount ()) * skill->GetT0 () *
                                                      10.001);
              skill->GetPlayer ()->SetPasadddefence (skill->GetLevel () * skill->GetT1 ());
              return true;
        }
#endif
    };
}
#endif
