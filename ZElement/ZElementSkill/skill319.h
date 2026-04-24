#ifndef __CPPGEN_GNET_SKILL319
#define __CPPGEN_GNET_SKILL319
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill319:public Skill
    {
      public:
        enum
        { SKILL_ID = 319 };
          Skill319 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill319Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill319Stub ():SkillStub (319)
        {
            occupation = 12;
            name = L"观音经";
            nativename = "观音经";
            icon = "观音经.dds";
            maxlevel = 12;
            maxlearn = 8;
            type = 2;
            eventflag = 0;
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
            skill_class = 3;
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
            talent[0] = 700;
            talent[1] = 714;
            talent_size = 2;
            action[0] = "天音寺_真言:定";
            action[1] = "天音寺_真言:定";
            action[2] = "天音寺_真言:定";
            action[3] = "天音寺_真言:定";
            action[4] = "天音寺_真言:定";
            action[5] = "天音寺_真言:定";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "天音寺_骑乘_真言:定";
            action[16] = "天音寺_骑乘_真言:定";
            action[17] = "天音寺_骑乘_真言:定";
            action[18] = "天音寺_骑乘_真言:定";
            action[19] = "天音寺_骑乘_真言:定";
            action[20] = "天音寺_骑乘_真言:定";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
            rangetype = 5;
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
        virtual ~ Skill319Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 282 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (135 + 30 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 10000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 80, 82, 86, 88, 92, 95, 97, 99 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               135 + 30 * skill->GetLevel () + 8 * skill->GetLevel () * skill->GetLevel (),
                               INT (18 * skill->GetLevel () + 0.2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()),
                               270 + 81 * skill->GetLevel () + 18 * skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (18000 + 100 + 4000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((18 * skill->GetLevel () + 0.2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.1 * skill->GetT0 ()));
            skill->GetVictim ()->SetAdddefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (18000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((270 + 81 * skill->GetLevel () + 18 * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.8 * skill->GetT1 ()));
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetRatio (220);
            skill->GetVictim ()->SetAmount (220 + 569 * norm (zrand (100) / (100 - 30 * skill->GetT0 ())));
            skill->GetVictim ()->SetValue (208);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
#endif
    };
}
#endif
