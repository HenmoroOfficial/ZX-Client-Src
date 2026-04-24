#ifndef __CPPGEN_GNET_SKILL3438
#define __CPPGEN_GNET_SKILL3438
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3438:public Skill
    {
      public:
        enum
        { SKILL_ID = 3438 };
          Skill3438 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3438Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 300;
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
      Skill3438Stub ():SkillStub (3438)
        {
            occupation = 66;
            name = L"ÄýÉñ°ÙÁ¶";
            nativename = "ÄýÉñ°ÙÁ¶";
            icon = "ÄýÉñ°ÙÁ¶.dds";
            maxlevel = 20;
            maxlearn = 9;
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
            talent[0] = 3401;
            talent[1] = 3405;
            talent_size = 2;
            action[0] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[1] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[2] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[3] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[4] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[5] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[6] = "·ÙÏã¹È_ÁéÁ¦Å²ÒÆ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÁéÁ¦Å²ÒÆ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
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
        virtual ~ Skill3438Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3437 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (332 + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 500;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 25000 - 10000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 95, 97, 99, 101, 103, 105, 108, 111, 115 };
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
            return _snwprintf (buffer, length, format, 332 + 3 * skill->GetLevel () * skill->GetLevel (), 15 + 6 * skill->GetLevel (), 0, 15);

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
            skill->GetVictim ()->
                SetTime ((INT (skill->GetPlayer ()->GetHasbuff (4199) == 1) + INT (skill->GetPlayer ()->GetHasbuff (4200) == 1) +
                          INT (skill->GetPlayer ()->GetHasbuff (4201) == 1) + INT (skill->GetPlayer ()->GetHasbuff (4202) == 1)) ==
                         1 ? 15 * 1000 : 1);
            skill->GetVictim ()->SetRatio (0.15 + 0.06 * skill->GetLevel () + 0.15 * skill->GetT1 ());
            skill->GetVictim ()->SetAmount (INT (skill->GetPlayer ()->GetHasbuff (4199) == 1) * 1 +
                                            INT (skill->GetPlayer ()->GetHasbuff (4200) == 1) * 2 + INT (skill->GetPlayer ()->GetHasbuff (4201) ==
                                                                                                         1) * 3 +
                                            INT (skill->GetPlayer ()->GetHasbuff (4202) == 1) * 0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetFocusanti (1);
            return true;
        }
#endif
    };
}
#endif
