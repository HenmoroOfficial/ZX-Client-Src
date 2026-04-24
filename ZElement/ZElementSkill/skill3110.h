#ifndef __CPPGEN_GNET_SKILL3110
#define __CPPGEN_GNET_SKILL3110
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3110:public Skill
    {
      public:
        enum
        { SKILL_ID = 3110 };
          Skill3110 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3110Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3110Stub ():SkillStub (3110)
        {
            occupation = 169;
            name = L"明镜灵台<禅>";
            nativename = "明镜灵台<禅>";
            icon = "佛·明镜灵台.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
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
            preskillnum = 1;
            action[0] = "青云门_明净灵台";
            action[1] = "青云门_明净灵台";
            action[2] = "鬼王宗_明净灵台";
            action[3] = "合欢派_明净灵台";
            action[4] = "天音寺_明净灵台";
            action[5] = "鬼道_明净灵台";
            action[6] = "焚香谷_明净灵台";
            action[7] = "兽神_明净灵台";
            action[8] = "狐妖_明净灵台";
            action[9] = "轩辕_明净灵台";
            action[10] = "怀光_明净灵台";
            action[11] = "太昊_明净灵台";
            action[12] = "天华_明净灵台";
            action[13] = "灵夙_明净灵台";
            action[14] = "英招_明净灵台";
            action[15] = "青云门_骑乘_明净灵台";
            action[16] = "青云门_骑乘_明净灵台";
            action[17] = "鬼王宗_骑乘_明净灵台";
            action[18] = "合欢派_骑乘_明净灵台";
            action[19] = "天音寺_骑乘_明净灵台";
            action[20] = "鬼道_骑乘_明净灵台";
            action[21] = "焚香谷_骑乘_明净灵台";
            action[22] = "兽神_骑乘_明净灵台";
            action[23] = "狐妖_骑乘_明净灵台";
            action[24] = "轩辕_骑乘_明净灵台";
            action[25] = "怀光_骑乘_明净灵台";
            action[26] = "太昊_骑乘_明净灵台";
            action[27] = "天华_骑乘_明净灵台";
            action[28] = "灵夙_骑乘_明净灵台";
            action[29] = "英招_骑乘_明净灵台";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
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
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3110Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1572 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1335 + 35 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 24000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1335 + 35 * skill->GetLevel (),
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               10 + skill->GetLevel (), 600 - 24 * skill->GetLevel (), 3 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (10000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (3 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 20);
            skill->GetVictim ()->SetAddanti (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (10);
        }
#endif
    };
}
#endif
