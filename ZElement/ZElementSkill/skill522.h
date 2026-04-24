#ifndef __CPPGEN_GNET_SKILL522
#define __CPPGEN_GNET_SKILL522
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill522:public Skill
    {
      public:
        enum
        { SKILL_ID = 522 };
          Skill522 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill522Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill522Stub ():SkillStub (522)
        {
            occupation = 13;
            name = L"明王降世";
            nativename = "明王降世";
            icon = "明王降世.dds";
            maxlevel = 20;
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
            talent[0] = 632;
            talent[1] = 649;
            talent_size = 2;
            action[0] = "鬼王宗_明王降世";
            action[1] = "鬼王宗_明王降世";
            action[2] = "鬼王宗_明王降世";
            action[3] = "鬼王宗_明王降世";
            action[4] = "鬼王宗_明王降世";
            action[5] = "鬼王宗_明王降世";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_明王降世";
            action[16] = "鬼王宗_骑乘_明王降世";
            action[17] = "鬼王宗_骑乘_明王降世";
            action[18] = "鬼王宗_骑乘_明王降世";
            action[19] = "鬼王宗_骑乘_明王降世";
            action[20] = "鬼王宗_骑乘_明王降世";
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
        virtual ~ Skill522Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 519 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (360 + 15 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 15000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 110, 112, 114, 116, 118, 120, 122, 124 };
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
            return _snwprintf (buffer, length, format, 360 + 15 * skill->GetLevel (), 18, 1 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (18000 + 2000 * skill->GetT1 () + 100);
            skill->GetVictim ()->SetValue (1 + 2 * skill->GetLevel () + 2 * skill->GetT0 () * skill->GetLevel ());
            skill->GetVictim ()->SetBlessed (1);
            skill->GetVictim ()->SetTime (2000 + 1500 * norm (skill->GetT0 () / 1.5));
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
#endif
    };
}
#endif
