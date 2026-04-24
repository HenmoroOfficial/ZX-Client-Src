#ifndef __CPPGEN_GNET_SKILL257
#define __CPPGEN_GNET_SKILL257
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill257:public Skill
    {
      public:
        enum
        { SKILL_ID = 257 };
          Skill257 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill257Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHp () * 1000 / skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill257Stub ():SkillStub (257)
        {
            occupation = 2;
            name = L"魔性大发";
            nativename = "魔性大发";
            icon = "魔性大发.dds";
            maxlevel = 12;
            maxlearn = 7;
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
            talent[0] = 642;
            talent[1] = 647;
            talent[2] = 662;
            talent[3] = 666;
            talent_size = 4;
            action[0] = "鬼王宗_横行无忌";
            action[1] = "鬼王宗_横行无忌";
            action[2] = "鬼王宗_横行无忌";
            action[3] = "鬼王宗_横行无忌";
            action[4] = "鬼王宗_横行无忌";
            action[5] = "鬼王宗_横行无忌";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_横行无忌";
            action[16] = "鬼王宗_骑乘_横行无忌";
            action[17] = "鬼王宗_骑乘_横行无忌";
            action[18] = "鬼王宗_骑乘_横行无忌";
            action[19] = "鬼王宗_骑乘_横行无忌";
            action[20] = "鬼王宗_骑乘_横行无忌";
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
            rangetype = 0;
            doenchant = true;
            dobless = true;
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
        virtual ~ Skill257Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 253 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.1 * skill->GetLevel () * skill->GetLevel () + 0.55 * skill->GetLevel () + 145);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 144000 - 18000 * skill->GetT2 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 55, 59, 63, 67, 71, 75, 79 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.1 * skill->GetLevel () * skill->GetLevel () + 0.55 * skill->GetLevel () + 145),
                               20, 20 + 10 * skill->GetLevel (), 18);

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
            skill->GetVictim ()->SetClearsilent (1);
            skill->GetVictim ()->SetTime (20000 + 100 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (10 * skill->GetLevel () + 20 + 4 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetAddantisilent (1);
            skill->GetVictim ()->SetProbability (1.0 * (10 * skill->GetT3 ()));
            skill->GetVictim ()->SetClearwrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT1 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (60000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.00001 + 0.1 * skill->GetT1 ());
            skill->GetVictim ()->SetValue (0.06 * skill->GetT1 () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetAddhp2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 950 ? 120 * norm (skill->GetT1 () / 3.9) : 0));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
